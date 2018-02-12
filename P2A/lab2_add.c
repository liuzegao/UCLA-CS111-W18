// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include <errno.h>          // number of last error
#include <string.h>         
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>         // Getopt long options
#include <time.h>           // clock and time
#include <pthread.h>        // posix threads


#define NO_LOCK             'n'
#define MUTEX               'm'
#define SPIN_LOCK           's'
#define COMPARE_AND_SWAP    'c'


// Parameter args
int opt_yield = 0;
int n_threads = 1;          // number of parallel threads (--threads=#, default 1)
int n_iterations = 1;       // number of iterations (--iterations=#, default 1)
char add_version = NO_LOCK; // 

// Globals
char test_name[20] = "add";
long long counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
int spin_lock = 0;


// Get-opt Long Options parsing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"yield", no_argument, &opt_yield, 1},
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "t:i:s:", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
            case 't':
                n_threads = atoi(optarg);
                break;
            case 'i':
                n_iterations = atoi(optarg);
                break;
            case 's':
                switch (optarg[0]){
                    case MUTEX:
                        add_version = MUTEX;
                        break;
                    case SPIN_LOCK:
                        add_version = SPIN_LOCK;
                        break;
                    case COMPARE_AND_SWAP:
                        add_version = COMPARE_AND_SWAP;
                        break;
                    default:
                        fprintf(stderr, "Error: unrecognized argument\n");
                        exit(1);
                }
                break;
            default:
                fprintf(stderr, "Error: unrecognized argument\n");
                exit(1);
        }
    }
}

void process_name(){
    if (opt_yield == 1){
        strcat(test_name, "-yield");
    }
    if (add_version != NO_LOCK){
        strcat(test_name, "-");
        strcat(test_name, &add_version);
    }
    else{
        strcat(test_name, "-none");
    }
}

// Extended basic add routine
void add(long long *pointer, long long value) {
        long long sum = *pointer + value;
        if (opt_yield)
            sched_yield();
        *pointer = sum;
}

/*
// Basic add routine that must not be modified
void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    *pointer = sum;
}
*/

void* add_sub_all(){
    long long old, new;
    for (long long val = -1; val <= 1; val += 2){
        for (int i = 0; i < n_iterations; i++){
            switch(add_version){
                case NO_LOCK:
                    add(&counter, val);
                    break;
                case MUTEX:
                    pthread_mutex_lock(&mutex);
                    add(&counter, val);
                    pthread_mutex_unlock(&mutex);
                    break;
                case SPIN_LOCK:
                    while(__sync_lock_test_and_set(&spin_lock, 1));
                    add(&counter, val);
                    __sync_lock_release(&spin_lock);
                    break;
                case COMPARE_AND_SWAP:
                    do {
                        if (opt_yield)
                            sched_yield();
                        old = counter;
                        new = old + val;
                    } while(__sync_val_compare_and_swap(&counter, old, new) != old);
                    break;
                default:
                    break;
            }
        }
    }
    return NULL;
}

int main(int argc, char **argv){
    // Process command line args
    process_args(argc, argv);
    process_name();

    // Malloc space for thread pointers
    pthread_t* thread_IDs = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    if (thread_IDs == NULL) {
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }

    // Record start time
    struct timespec start_time;
    if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1){
        fprintf(stderr, "Error: clock_gettime failed\n%s\n", strerror(errno));
        exit(1);
    }

    // Starts the specified number of threads
    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&thread_IDs[i], NULL, add_sub_all, NULL) != 0) {
            fprintf(stderr, "Error: pthread_create failed\n");
            free(thread_IDs);
            exit(1);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < n_threads; i++) {
        if (pthread_join(thread_IDs[i], NULL) != 0) {
            fprintf(stderr, "Error: pthread_join failed\n");
            exit(1);
        }
    }
    free(thread_IDs);

    // Record end time
    struct timespec end_time;
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) == -1){
        fprintf(stderr, "Error: clock_gettime failed\n%s\n", strerror(errno));
        exit(1);
    }

    // Calculate elapsed time
    long long elapsed_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000;
    elapsed_time_ns += end_time.tv_nsec;
    elapsed_time_ns -= start_time.tv_nsec;

    // Calculate stats
    long long n_operations = n_threads * n_iterations * 2;
    long long time_per_operation = elapsed_time_ns / n_operations;

    // Prints to stdout a comma-separated-value
    printf("%s,%d,%d,%lld,%lld,%lld,%lld\n", test_name, n_threads, n_iterations, n_operations, elapsed_time_ns, time_per_operation, counter);

    exit(0);//deal with freeing shit
}
