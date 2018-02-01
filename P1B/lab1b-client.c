// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322


#include <errno.h> //strerror
#include <fcntl.h> // open
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h> // poll
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> // affect terminal
#include <unistd.h> // close, dup, read
#include "zlib.h"


// Constants
#define BUFF_SIZE 1024
int READ_SIZE = 256;
unsigned char CRLF[2] = {'\r', '\n'};
unsigned char LF[1] = {'\n'}; 

// Process args
int port_num;
int fd_log;
int log_flag = 0;
static int compress_flag = 0;

// Globals
int fd_socket;
struct termios init_terminal_attr;

void create_outfile(char* out_file){
    fd_log = creat(out_file, 0666);
    if (fd_log < 0) {
        fprintf(stderr, "Cannot open output file '%s'\n", out_file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }
}

// get-opt long arg processing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"port", required_argument, 0, 'p'},
        {"log", required_argument, 0, 'l'},
        {"compress", no_argument, &compress_flag, 1}, // flag
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:l:", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
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

// poll read from fd_socket; poll read from fd_read
// write to fd_write from fd_read; write to fd_write from fd_socket
void read_write_socket(int fd_read, int fd_write){ 
    struct pollfd fds[2];
    fds[0].fd = fd_read;  
    fds[1].fd = fd_socket;
    fds[0].events = POLLIN | POLLHUP | POLLERR;
    fds[1].events = POLLIN | POLLHUP | POLLERR;

    while (1){
        int nrevents = poll(fds, 2, 0);

        if (nrevents == 0){
            continue;
        }
        else if (nrevents < 0){
            fprintf(stderr, "Error: polling failed\n%s\n", strerror(errno));
            exit(1);
        }
        else if (fds[0].revents & POLLIN){
            unsigned char buffer[READ_SIZE];
            char buffer_line_send[READ_SIZE];
            int num_bytes = read(fds[0].fd, buffer, READ_SIZE);

            for (int i = 0; i < num_bytes; i++){
                switch (*(buffer+i)){
                    case '\n':
                    case '\r':
                        write(fd_write, &CRLF, 2);
                        if (compress_flag){
                            unsigned long size = 1024;
                            unsigned char out[READ_SIZE];
                            compress(out, &size, LF, 1);
                            write(fd_socket, out, size);

                            if (log_flag){
                                sprintf(buffer_line_send, "SENT %lu bytes: ", size);
                                write(fd_log, buffer_line_send, strlen(buffer_line_send));
                                write(fd_log, out, sizeof(char) * size);
                                write(fd_log, "\n", sizeof(char));
                            }
                        }
                        else{
                            write(fd_socket, &LF, 1);

                            if (log_flag){
                                sprintf(buffer_line_send, "SENT %d bytes: ", 1);
                                write(fd_log, buffer_line_send, strlen(buffer_line_send));
                                write(fd_log, LF, sizeof(char) * 1);
                                write(fd_log, "\n", sizeof(char));
                            }
                        }
                        break;
                    default:
                        write(fd_write, buffer + i, 1);
                        if (compress_flag){
                            unsigned long size = 1024;
                            unsigned char out[READ_SIZE];
                            compress(out, &size, buffer + i, 1);
                            write(fd_socket, out, size);

                            if (log_flag){
                                sprintf(buffer_line_send, "SENT %lu bytes: ", size);
                                write(fd_log, buffer_line_send, strlen(buffer_line_send));
                                write(fd_log, out, sizeof(char) * size);
                                write(fd_log, "\n", sizeof(char));
                            }
                        }
                        else{
                            write(fd_socket, buffer + i, 1);

                            if (log_flag){
                                sprintf(buffer_line_send, "SENT %d bytes: ", 1);
                                write(fd_log, buffer_line_send, strlen(buffer_line_send));
                                write(fd_log, buffer + i, sizeof(char) * 1);
                                write(fd_log, "\n", sizeof(char));
                            }
                        }
                        break;
                }
            }
        }
        else if (fds[1].revents & POLLIN){
            unsigned char buffer[READ_SIZE];
            char buffer_line_receive[READ_SIZE];
            int num_bytes = read(fds[1].fd, buffer, READ_SIZE);
            if (num_bytes <= 0)
                exit(0);

            if (log_flag){
                sprintf(buffer_line_receive, "RECEIVED %d bytes: ", num_bytes);
                write(fd_log, buffer_line_receive, strlen(buffer_line_receive));
                write(fd_log, buffer, sizeof(char) * num_bytes);
                write(fd_log, "\n", sizeof(char));
            }

            if (compress_flag){
                unsigned long size = 1024;
                unsigned char out[READ_SIZE];

                uncompress(out, &size, buffer, num_bytes);
                num_bytes = size;
                strcpy((char*) buffer, (char*) out);
            }

            for (int i = 0; i < num_bytes; i++){
                switch (*(buffer + i)){
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
        else if (fds[1].revents & (POLLHUP | POLLERR)){
            close(fd_socket);
            exit(0);
        }
    }
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
