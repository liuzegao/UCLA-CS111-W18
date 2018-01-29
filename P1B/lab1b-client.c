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
#include <termios.h> // affect terminal
#include <unistd.h> // close, dup, read

// Constants
int READ_SIZE = 256;
char CRLF[2] = {'\r', '\n'};
char LF[1] = {'\n'}; 

// Process args
int port_num;
int fd_log;
int log_flag = 0;
int compress;

// Globals
int fd_socket;
struct termios init_terminal_attr;

// get-opt long arg processing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"compress", no_argument, &compress, 1}, // flag
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:l:c", long_options, NULL)) != -1){
        switch (opt){
            case 'p':
                port_num = atoi(optarg);
                break;
            case 'l':
                log_flag = 1;
                create_outfile(optarg);
                break;
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }
}

void create_outfile(char* out_file){
    fd_log = creat(out_file, S_IRWXU);
    if (fd_log < 0) {
        fprintf(stderr, "Cannot open output file '%s'\n", out_file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(3);
    }
}

// check stdin is terminal
void check_terminal(void){
    if (!isatty(STDIN_FILENO)){
        fprintf(stderr, "Error: stdin is not a terminal\n%s\n", strerror(errno));
        exit(1);
    }
}
 
// reset terminal to original state
void reset_terminal(void){
    tcsetattr(STDIN_FILENO, TCSANOW, &init_terminal_attr);
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

// poll read from fd_socket; poll read from fd_read
// write to fd_write from fd_read; write to fd_write from fd_socket
void read_write_socket(int fd_read, int fd_write){ 
    struct pollfd fds[2];
    fds[0].fd = fd_read;  
    fds[1].fd = fd_socket;
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
            if (fds[0].revents & POLLIN){
                num_bytes = read(fds[0].fd, buffer, READ_SIZE);
                for (int i = 0; i < num_bytes; i++){
                    switch (*(buffer+i)){
                        case '\n':
                        case '\r':
                            write(fd_write, &CRLF, 2);
                            write(fd_socket, &LF, 1);
                            break;
                        default:
                            write(fd_write, buffer + i, 1);
                            write(fd_socket, buffer + i, 1);
                            break;
                    }
                }
                if (log_flag){
                    char buffer_to_file[num_bytes + 20];
                    sprintf(buffer_to_file, "SENT %d bytes: %s\n", (int) strlen(buffer), buffer);
                    write(fd_log, buffer_to_file, strlen(buffer_to_file));
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
                /*if (log_flag){
                    char buffer_to_file[num_bytes + 20];
                    sprintf(buffer_to_file, "RECEIVED %d bytes: %s\n", (int) strlen(buffer), buffer);
                    write(fd_log, buffer_to_file, strlen(buffer_to_file));
                }*/
            }

            if (fds[1].revents & (POLLHUP | POLLERR)){
                exit(0);
            }
        }

    }
    free(buffer);
}

// connect to server at localhost port#
void connect_to_server(){
    struct sockaddr_in server_addr;
    struct hostent *server;

    if ((server = gethostbyname("localhost")) == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
    memset((char *) &server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memmove((char *) &server_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);
    server_addr.sin_port = htons(port_num);
    if (connect(fd_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
}

int main(int argc, char **argv){
    process_args(argc, argv);

    check_terminal();
    save_terminal();
    set_terminal();

    connect_to_server();
    
    read_write_socket(0, 1);

    exit(0);
}
