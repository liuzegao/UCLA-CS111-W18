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
#include "zlib.h"


// Constants
#define BUFF_SIZE 1024
int READ_SIZE = 256;
char CRLF[2] = {'\r', '\n'};
char LF[1] = {'\n'}; 

// Process args
int port_num;
static int compress_flag = 0;

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
        {"compress", no_argument, &compress_flag, 1}, // flag
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
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

int def(unsigned char in[BUFF_SIZE], unsigned char out[BUFF_SIZE]){
    int ret;
    //unsigned have;
    z_stream stdin_to_shell;

    stdin_to_shell.zalloc = Z_NULL;
    stdin_to_shell.zfree = Z_NULL;
    stdin_to_shell.opaque = Z_NULL;

    ret = deflateInit(&stdin_to_shell, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        return ret;

    stdin_to_shell.avail_in = 2;
    stdin_to_shell.next_in = in;
    stdin_to_shell.avail_out = BUFF_SIZE;
    stdin_to_shell.next_out = out;
    do {
        deflate(&stdin_to_shell, Z_SYNC_FLUSH);
    } while(stdin_to_shell.avail_in > 0);

    deflateEnd(&stdin_to_shell);
    return BUFF_SIZE - stdin_to_shell.avail_out;
}

int inf(unsigned char in[BUFF_SIZE], unsigned char out[BUFF_SIZE]) {
    int ret;
    z_stream shell_to_stdout;

    shell_to_stdout.zalloc = Z_NULL;
    shell_to_stdout.zfree = Z_NULL;
    shell_to_stdout.opaque = Z_NULL;

    ret = inflateInit(&shell_to_stdout);
    if (ret != Z_OK)
        return ret;

    shell_to_stdout.avail_in = strlen((char*) in);
    shell_to_stdout.next_in = in;
    shell_to_stdout.avail_out = BUFF_SIZE;
    shell_to_stdout.next_out = out;
    do {
        inflate(&shell_to_stdout, Z_SYNC_FLUSH);
    } while(shell_to_stdout.avail_in > 0);

    inflateEnd(&shell_to_stdout);
    return shell_to_stdout.total_out;
}

// write to shell from fd_client; wriet to fd_client from shell
void redirect(int fd_client){
    struct pollfd fds[2];
    fds[0].fd = fd_client; 
    fds[1].fd = pipe_to_server[0]; // input to server from shell
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;

    while (1){
        int nrevents = poll(fds, 2, 0);

        if (nrevents == 0){
            continue;
        }
        if (nrevents < 0){
            fprintf(stderr, "Error: polling failed\n%s\n", strerror(errno));
            exit(1);
        }
        if (fds[0].revents & POLLIN){ // send to shell
            unsigned char buffer[READ_SIZE];
            int num_bytes = read(fds[0].fd, buffer, READ_SIZE);
            if (compress_flag){
                unsigned long size = 1024;
                unsigned char out[READ_SIZE];

                uncompress(out, &size, buffer, num_bytes);
                num_bytes = size;
                strcpy((char*) buffer, (char*) out);

                //fprintf(stderr, "196:%s:196\n", out);
                //fprintf(stderr, "197:%d:197\n", num_bytes);
            }

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
            unsigned char buffer[READ_SIZE];
            int num_bytes = read(fds[1].fd, buffer, READ_SIZE);

            if (compress_flag){
                unsigned long size = 1024;
                unsigned char out[READ_SIZE];
                compress(out, &size, buffer, num_bytes);
                write(fd_client, out, size);
            }
            else{
                write(fd_client, buffer, num_bytes);
            }

            /*
            for (int i = 0; i < num_bytes; i++){
                switch (*(buffer + i)){
                    default:
                        if (compress_flag){
                            unsigned long size = 1024;
                            unsigned char out[READ_SIZE];
                            compress(out, &size, buffer + i, 1);
                            write(fd_client, out, size);
                        }
                        else{
                            write(fd_client, buffer + i, 1);
                        }
                        break;
                }
            }*/
        }
        if (fds[1].revents & (POLLHUP | POLLERR)){
            int status = 0;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", status & 0x007f, WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));

            close(fd_client);
            close(fd_socket);
            exit(0);
        }
    }
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
