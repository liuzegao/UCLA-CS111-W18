// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include <errno.h> //strerror
#include <fcntl.h> // open
#include <getopt.h>
#include <poll.h> // poll
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // pipe
#include <sys/wait.h> // waitpid
#include <termios.h> // affect terminal
#include <unistd.h> // close, dup, read

int READ_SIZE = 256;
char CRLF[2] = {'\r', '\n'};
char LF[1] = {'\n'}; 

static int shell = 0;

struct termios init_terminal_attr;

int pipe_to_child[2];
int pipe_to_parent[2];

pid_t pid;

// check stdin is terminal
void check_terminal(void){
    if (!isatty(STDIN_FILENO)){
        fprintf(stderr, "Error: stdin is not a terminal\n%s\n", strerror(errno));
        exit(1);
    }
}

void reset_terminal(void){
    tcsetattr(STDIN_FILENO, TCSANOW, &init_terminal_attr);
    int status = 0;
    waitpid(pid, &status, 0);
    if (shell){
        if (WIFEXITED(status))
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", status & 0x007f, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
    }
}

// save terminal attributes and prepare exit handler
void save_terminal(void){
    tcgetattr(STDIN_FILENO, &init_terminal_attr);
    atexit(reset_terminal);
}

// set noncanonical input no echo with additonal flagged settings
void set_terminal(void){
    struct termios ncine; 

    // clear ICANON and ECHO
    tcgetattr(STDIN_FILENO, &ncine);
    ncine.c_iflag = ISTRIP;   /* only lower 7 bits    */
    ncine.c_oflag = 0;        /* no processing        */
    ncine.c_lflag = 0;        /* no processing        */
    ncine.c_cc[VMIN] = 1;
    ncine.c_cc[VTIME] = 0;

    // set new terminal mode immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &ncine);
}

void sigint_handler(int signum){
    signum++;
    fprintf(stderr, "fffff");
    char out[] = "Caught signal\n";
    write(2, out, sizeof(out) / sizeof(char));
    kill(pid, SIGINT);
}

void sigpipe_handler(int signum){
    signum++;
    fprintf(stderr, "111111");
    char out[] = "Caught signal\n";
    write(2, out, sizeof(out) / sizeof(char));
    exit(0);
}

void read_write(int fd_read, int fd_write){
    char* buffer;
    buffer = malloc(READ_SIZE);
    int num_bytes = 0;

    while((num_bytes = read(fd_read, buffer, READ_SIZE)) > 0){
        for (int i = 0; i < num_bytes; i++){
            switch (*(buffer + i)){
                case 0x04:
                    exit(0);
                    break;
                case '\n':
                case '\r':
                    write(fd_write, &CRLF, 2);
                    break;
                default:
                    write(fd_write, buffer + i, 1);
                    break;
            }
        }
    }
    free(buffer);
}

void read_write_parent(int fd_read, int fd_write){ // parents version of read_write
    struct pollfd fds[2];
    fds[0].fd = fd_read; // stdin 
    fds[1].fd = pipe_to_parent[0]; // input to parent from child
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;

    char* buffer;
    buffer = malloc(READ_SIZE);
    int num_bytes = 0;

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
            if (fds[0].revents & POLLIN){
                num_bytes = read(fds[0].fd, buffer, READ_SIZE);
                for (int i = 0; i < num_bytes; i++){
                    switch (*(buffer+i)){
                        case 0x03:
                            kill(pid, SIGINT);
                            break;
                        case 0x04:
                            close(pipe_to_child[1]);
                            break;
                        case '\n':
                        case '\r':
                            write(fd_write, &CRLF, 2);
                            write(pipe_to_child[1], &LF, 1);
                            break;
                        default:
                            write(fd_write, buffer + i, 1);
                            write(pipe_to_child[1], buffer + i, 1);
                            break;
                    }
                }
            }

            if (fds[1].revents & POLLIN){
                num_bytes = read(fds[1].fd, buffer, READ_SIZE);
                for (int i = 0; i < num_bytes; i++){
                    switch (*(buffer + i)){
                        case '\n':
                            write(fd_write, &CRLF, 2);
                            break;
                        default:
                            write(fd_write, buffer + i, 1);
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

void create_pipe(int p[2]){
    if (pipe(p) == -1) {
        fprintf(stderr, "Error: pipe failed\n%s\n", strerror(errno));
        exit(1);
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
        close(pipe_to_child[1]);
        close(pipe_to_parent[0]);
        dup2(pipe_to_child[0], 0);
        dup2(pipe_to_parent[1], 1);
        dup2(pipe_to_parent[1], 2);
        close(pipe_to_child[0]);
        close(pipe_to_parent[1]);

        char* argv[2];
        argv[0] = "/bin/bash";
        argv[1] = NULL;

        if (execvp(argv[0], argv) == -1){
            fprintf(stderr, "Error: execvp failed\n%s\n", strerror(errno));
            exit(1);
        }
    }
    else{ // parent
        close(pipe_to_child[0]);
        close(pipe_to_parent[1]);

        read_write_parent(0, 1);
    }
}

int main(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"shell", no_argument, &shell, 1}, // flag
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
                
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }

    check_terminal();
    save_terminal();
    set_terminal();

    create_pipe(pipe_to_child);
    create_pipe(pipe_to_parent);

    if (shell)
        exec_shell();
    else
        read_write(0, 1);

    exit(0);
}
