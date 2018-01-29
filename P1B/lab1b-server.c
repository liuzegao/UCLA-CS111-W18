// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include <errno.h> //strerror
#include <fcntl.h> // open
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h> // poll
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // pipe
#include <sys/wait.h> // waitpid
#include <termios.h> // affect terminal
#include <unistd.h> // close, dup, read


// Constants
int READ_SIZE = 256;
char CRLF[2] = {'\r', '\n'};
char LF[1] = {'\n'}; 

// Process args
int port_num;
int compress;

// Globals
int pipe_to_shell[2];
int pipe_to_server[2];
pid_t pid;
int fd_socket, fd_client;


// get-opt long arg processing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"compress", no_argument, &compress, 1}, // flag
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:c", long_options, NULL)) != -1){
        switch (opt){
            case 'p':
                port_num = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }
}

// start the server and check for error
void start_server(){
    struct sockaddr_in server_addr;

    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    memset((char *) &server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_num);
    if (bind(fd_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }
}

// accept a client and check for error
void accept_client(){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((fd_client = accept(fd_socket, (struct sockaddr *) &client_addr, &client_len)) < 0) {
        perror("ERROR on accept");
        exit(1);
    }
}

// init a pipe and check for error
void create_pipe(int p[2]){
    if (pipe(p) == -1) {
        fprintf(stderr, "Error: pipe failed\n%s\n", strerror(errno));
        exit(1);
    }
}

void sigint_handler(int signum){
    signum++;
    char out[] = "Caught signal\n";
    write(2, out, sizeof(out) / sizeof(char));
    kill(pid, SIGINT);
}

void sigpipe_handler(int signum){
    signum++;
    char out[] = "Caught signal\n";
    write(2, out, sizeof(out) / sizeof(char));
    exit(0);
}

// write to shell from fd_client; wriet to fd_client from shell
void redirect(int fd_client){
    struct pollfd fds[2];
    fds[0].fd = fd_client; 
    fds[1].fd = pipe_to_server[0]; // input to server from shell
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;

    int num_bytes;
    char* buffer;
    buffer = malloc(READ_SIZE);

    while (1){
        int nrevents = poll(fds, 2, 0);

        if (nrevents == 0){
            continue;
        }
        else if (nrevents < 0){
            fprintf(stderr, "Error: polling failed\n%s\n", strerror(errno));
            exit(1);
        }
        else{ 
            if (fds[0].revents & POLLIN){ // send to shell
                num_bytes = read(fds[0].fd, buffer, READ_SIZE);
                for (int i = 0; i < num_bytes; i++){
                    switch (*(buffer+i)){
                        case 0x03:
                            kill(pid, SIGINT);
                            break;
                        case 0x04:
                            close(pipe_to_shell[1]);
                            break;
                        case '\n':
                        case '\r':
                            write(pipe_to_shell[1], &LF, 1);
                            break;
                        default:
                            write(pipe_to_shell[1], buffer + i, 1);
                            break;
                    }
                }
            }

            if (fds[1].revents & POLLIN){ // send to client
                num_bytes = read(fds[1].fd, buffer, READ_SIZE);
                for (int i = 0; i < num_bytes; i++){
                    switch (*(buffer + i)){
                        case '\n':
                            write(fd_client, &CRLF, 2);
                            break;
                        default:
                            write(fd_client, buffer + i, 1);
                            break;
                    }
                }
            }

            if (fds[1].revents & (POLLHUP | POLLERR)){
                exit(0);
            }
        }

    }
    free(buffer);
}

// turn child process into bash shell with appropriate environment changes
void exec_shell(void){
    signal(SIGINT, sigint_handler);
    signal(SIGPIPE, sigpipe_handler);
    pid = fork();

    if (pid == -1){ // fail
        fprintf(stderr, "Error: fork failed\n%s\n", strerror(errno));
        exit(1);
    }
    else if (pid == 0){ // child
        close(pipe_to_shell[1]); // close 1 if you want to listen
        close(pipe_to_server[0]); // close 0 if you want to talk
        dup2(pipe_to_shell[0], 0);
        dup2(pipe_to_server[1], 1);
        dup2(pipe_to_server[1], 2);
        close(pipe_to_shell[0]);
        close(pipe_to_server[1]);

        char* argv[2];
        argv[0] = "/bin/bash";
        argv[1] = NULL;

        if (execvp(argv[0], argv) == -1){
            fprintf(stderr, "Error: execvp failed\n%s\n", strerror(errno));
            exit(1);
        }
    }
    else{ // parent
        close(pipe_to_shell[0]);
        close(pipe_to_server[1]);
    }
}

int main(int argc, char **argv){
    process_args(argc, argv);

    start_server();
    listen(fd_socket, 5);
    accept_client();
    
    create_pipe(pipe_to_shell);
    create_pipe(pipe_to_server);

    exec_shell();

    redirect(fd_client);

    exit(0);
}