// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <getopt.h>
# include <poll.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/errno.h>
# include <time.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <ctype.h>
# include <fcntl.h> 
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <openssl/ssl.h>

# include <mraa/aio.h>
# include <mraa/gpio.h>


// Command-line arguments
int period = 1;     // default = 1
char scale = 'F';   // default = F
int log_fd = -1;    
char* student_id = "000000000";
char* host = "localhost";
int port = -1;

// Globals
int enabled = 1; // 0 = disabled; 1 = enabled

// Globals for server
struct sockaddr_in serv_addr;
int socket_fd;
int command_fd;
int report_fd;
SSL* ssl_client = 0;
SSL_CTX* ssl_context;
// Globals for sensor
mraa_aio_context temp_sensor;


// Get-opt Long Options parsing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"period", required_argument, 0, 'p'},
        {"scale", required_argument, 0, 's'},
        {"log", required_argument, 0, 'l'},
        {"id", required_argument, 0, 'i'},
        {"host", required_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:s:l:i:h:", long_options, NULL)) != -1){
        switch (opt){
            case 'p':
                period = atoi(optarg);
                if (period <= 0){
                    fprintf(stderr, "Error: invalid period time\n");
                    exit(1);
                }
                break;
            case 's':
                scale = optarg[0];
                if (optarg[0] == 'C')
                    scale = 'C';
                else if (optarg[0] == 'F')
                    scale = 'F';
                else{
                    fprintf(stderr, "Error: invalid scale\n");
                    exit(1);
                }
                break;
            case 'l':
                log_fd = creat(optarg, 0666);
                if (log_fd <= 0) {
                    fprintf(stderr, "Error: creat failed\n");
                    exit(1);
                }
                break;
            case 'i':
                if (strlen(optarg) == 9)
                    student_id = optarg;
                else{
                    fprintf(stderr, "Error: ID has the incorrect number of digits\n");
                    exit(1);
                }
                break;
            case 'h':
                host = optarg;
                break;
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }

    if (optind < argc){
        port = atoi(argv[optind]);
        if (port <= 0){
            fprintf(stderr, "Error: invalid port\n");
        }
    }
    else{
        fprintf(stderr, "Error: missing required port\n");
        exit(1);
    }

}

void open_tcp_connection(){
    struct hostent *server;
    struct sockaddr_in;
    int optval = 1;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    server = gethostbyname(host);
    if (server == NULL){
        //fprintf(stderr, "Error: cannt find address for host asdfasdf", host);
        exit(1);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr,
          (char*) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    int ret = connect(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if (ret < 0){
        fprintf(stderr, "Error: tcp connect() failed\n");
    }
    command_fd = socket_fd;
    report_fd = socket_fd;
}

void send_id(){
    char msg[13];
    sprintf(msg, "ID=%s\n", student_id);
    if (SSL_write(ssl_client, msg, 13) < 0){
        fprintf(stderr, "Error: SSL_write() failed\n");
        exit(2);
    }
}

// Read temperature from sensor and convert to C or F
float read_temp(){
    int reading = mraa_aio_read(temp_sensor);

    int B = 4275;
    float R0 = 100000.0;
    float R = 1023.0/((float) reading) - 1.0;
    R = R0 * R;
    float C = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;

    return (scale == 'F') ? (C * 9)/5 + 32 : C;
}

void write_report(char* out){
    struct timeval clock;
    gettimeofday(&clock, 0);
    struct tm* now = localtime(&(clock.tv_sec));

    char buffer[256] = {0};
    sprintf(buffer, "%02d:%02d:%02d %s\n", now->tm_hour, now->tm_min, now->tm_sec, out);
    write(1, buffer, strlen(buffer));
    SSL_write(ssl_client, buffer, strlen(buffer));

    if (log_fd != -1)
        write(log_fd, buffer, strlen(buffer));
}

void sample(){
    struct pollfd fds[1];
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN;

    while (1){
        // Sample temperature and create report
        if (enabled){
            float temperature = read_temp();
            char out[256] = {0};
            sprintf(out, "%.1f", temperature);
            write_report(out);
        }

        // Wait for # period seconds
        sleep(period);

        // Poll stdin for commands
        int nrevents = poll(fds, 1, 0);
        if (nrevents == 0){
            continue;
        }
        else if (nrevents < 0){
            fprintf(stderr, "Error: polling failed\n%s\n", strerror(errno));
            exit(1);
        }
        else if (fds[0].revents & POLLIN){
            char buffer[1024] = {0};
            //read(fds[0].fd, buffer, 1024);
            SSL_read(ssl_client, buffer, 1024);
            char *token = strtok(buffer, "\n");

            while(token) {
                if (!strncmp(token, "SCALE=F", 7)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));

                    scale = 'F';
                }
                else if (!strncmp(token, "SCALE=C", 7)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));
                    
                    scale = 'C';
                }
                else if (!strncmp(token, "PERIOD=", 7)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));

                    period = atoi(token + 7);
                    if (period <= 0){
                        fprintf(stderr, "Error: invalid period time\n");
                        exit(1);
                    }
                }
                else if (!strncmp(token, "STOP", 4)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));

                    enabled = 0;
                }
                else if (!strncmp(token, "START", 5)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));

                    enabled = 1;
                }
                else if (!strncmp(token, "LOG", 3)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));
                }
                else if (!strncmp(token, "OFF", 3)){
                    if (log_fd != -1)
                        write(log_fd, token, strlen(token));
                        
                    write_report("SHUTDOWN");
                    exit(0);
                }
                else{
                    fprintf(stderr, "Error: invalid command: %s\n", token);
                    exit(1);
                }
                if (log_fd != -1)
                    write(log_fd, "\n", 1);
                token = strtok(NULL, "\n");
            }
        }
    }
}

// Exit handler
void atexit_handler(){
    mraa_aio_close(temp_sensor);
    close(log_fd);
    SSL_shutdown(ssl_client);
    SSL_free(ssl_client);
    close(command_fd);
}

int main(int argc, char **argv){
    // Process command line args
    process_args(argc, argv);

    fprintf(stderr, "period=%d\n", period);
    fprintf(stderr, "scale=%c\n", scale);
    fprintf(stderr, "log_fd=%d\n", log_fd);
    fprintf(stderr, "id=%s\n", student_id);
    fprintf(stderr, "host=%s\n", host);
    fprintf(stderr, "port=%d\n", port);

    // Open TCP       
    open_tcp_connection();

    fprintf(stderr, "1\n");

    if (SSL_library_init() < 0){
        fprintf(stderr, "Error: SSL_library_init() failed\n");
        exit(2);
    }
    fprintf(stderr, "2\n");
    SSL_load_error_strings();
    fprintf(stderr, "3\n");
    OpenSSL_add_all_algorithms();
    fprintf(stderr, "4\n");
    ssl_context = SSL_CTX_new(TLSv1_client_method());
    fprintf(stderr, "5\n");
    if (ssl_context == NULL){
        fprintf(stderr, "Error: ssl_context failed\n");
        exit(2);
    }
    fprintf(stderr, "6\n");
    ssl_client = SSL_new(ssl_context);
    fprintf(stderr, "7\n");
    if (ssl_client == NULL){
        fprintf(stderr, "Error: SSL_new() failed\n");
        exit(2);
    }
    fprintf(stderr, "8\n");
    if (!SSL_set_fd(ssl_client, command_fd)){
        fprintf(stderr, "Error: SSL_set_fd() failed\n");
        exit(2);
    }
    fprintf(stderr, "9\n");
    if (SSL_connect(ssl_client) != 1){
        fprintf(stderr, "Error: SSL_connect() failed\n");
        exit(2);
    }
    fprintf(stderr, "10\n");

    send_id();
    fprintf(stderr, "11\n");
    // Setup the shutdown_flag sequence
    atexit(atexit_handler);

    // Initialize sensors
    temp_sensor = mraa_aio_init(1);
    
    // Begin sample loop
    sample();

    exit(0);
}
