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

#include <mraa/aio.h>
#include <mraa/gpio.h>

int period = 1;
char scale = 'F';
int log_fd = -1;

mraa_aio_context temp_sensor;
mraa_gpio_context button;

int shutdown = 0; // 
int enabled = 1; // 0 = disabled; 1 = enabled
int verbose;


// Get-opt Long Options parsing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"period", required_argument, 0, 'p'},
        {"scale", required_argument, 0, 's'},
        {"log", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "p:s:l:", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
            case 'p':
                period = atoi(optarg);
                if (period == 0){
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
                if (log_fd < 0) {
                    fprintf(stderr, "Error: creat failed\n");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }
}

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

    if (log_fd != -1)
        write(log_fd, buffer, strlen(buffer));
}

void sample(){
    struct pollfd fds[1];
    fds[0].fd = 0;
    fds[0].events = POLLIN;

    while (1){
        // Sample temperature and create report
        if (enabled){
            float temperature = read_temp();
            char out[256] = {0};
            sprintf(out, "%.1f", temperature);
            write_report(out);
        }

        // Sample button
        if (mraa_gpio_read(button)){
            write_report("SHUTDOWN");
            exit(1);
        }

        // Poll stdin for commands
        int nrevents = poll(fds, 1, period * 1000);
        if (nrevents == 0){
            continue;
        }
        else if (nrevents < 0){
            fprintf(stderr, "Error: polling failed\n%s\n", strerror(errno));
            exit(1);
        }
        else if (fds[0].revents & POLLIN){
            char buffer[256] = {0};
            read(fds[0].fd, buffer, 256);

            if (!strncmp(buffer, "SCALE=F", 7)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                scale = 'F';
            }
            else if (!strncmp(buffer, "SCALE=C", 7)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                scale = 'C';
            }
            else if (!strncmp(buffer, "PERIOD=", 7)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                period = atoi(buffer + 7);
                if (period == 0){
                    fprintf(stderr, "Error: invalid period time\n");
                    exit(1);
                }
            }
            else if (!strncmp(buffer, "STOP", 4)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                enabled = 0;
            }
            else if (!strncmp(buffer, "START", 5)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                enabled = 1;
            }
            else if (!strncmp(buffer, "LOG", 3)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
            }
            else if (!strncmp(buffer, "OFF", 3)){
                if (log_fd != -1)
                    write(log_fd, buffer, strlen(buffer));
                write_report("SHUTDOWN");
                exit(1);
            }
            else{
                fprintf(stderr, "Error: invalid command\n");
                exit(1);
            }
        }
    }
}

void atexit_handler(){
    mraa_gpio_close(button);
    mraa_aio_close(temp_sensor);
    //free shit
}

int main(int argc, char **argv){
    // Process command line args
    process_args(argc, argv);

    // Setup the shutdown sequence
    atexit(atexit_handler);

    // Initialize sensors
    temp_sensor = mraa_aio_init(1);
    button = mraa_gpio_init(62);
    mraa_gpio_dir(button, MRAA_GPIO_IN);
    
    // Begin sample loop
    sample();

    exit(0);
}