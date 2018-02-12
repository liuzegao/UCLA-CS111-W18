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
#include "SortedList.h"
#include <signal.h>


#define NO_LOCK             'n'
#define MUTEX               'm'
#define SPIN_LOCK           's'


// Parameter args
int opt_yield = 0;
int n_threads = 1;          // number of parallel threads (--threads=#, default 1)
int n_iterations = 1;       // number of iterations (--iterations=#, default 1)
char add_version = NO_LOCK; // 

// Globals
char test_name[20] = "list";
pthread_t* thread_IDs;
long long n_list_elements = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
int spin_lock = 0;
SortedList_t* list;
SortedListElement_t* list_elements;
long long* start_pos;

// Get-opt Long Options parsing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "t:i:y:s:", long_options, NULL)) != -1){
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
            case 'y':
                for (int i = 0; i < (int) strlen(optarg); i++){
                    switch (optarg[i]){
                        case 'i':
                            opt_yield |= INSERT_YIELD;
                            break;
                        case 'd':
                            opt_yield |= DELETE_YIELD;
                            break;
                        case 'l':
                            opt_yield |= LOOKUP_YIELD;
                            break;
                        default:
                            fprintf(stderr, "Error: unrecognized argument\n");
                            exit(1);
                    }
                }
                break;
            case 's':
                switch (optarg[0]){
                    case MUTEX:
                        add_version = MUTEX;
                        break;
                    case SPIN_LOCK:
                        add_version = SPIN_LOCK;
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
    if (opt_yield == 0){
        strcat(test_name, "-none");
    }
    else{
        strcat(test_name, "-");
        if (opt_yield & INSERT_YIELD){
            strcat(test_name, "i");
        }
        if (opt_yield & DELETE_YIELD){
            strcat(test_name, "d");
        }
        if (opt_yield & LOOKUP_YIELD){
            strcat(test_name, "l");
        }
    }

    if (add_version == SPIN_LOCK){
        strcat(test_name, "-s");
    }
    else if (add_version == MUTEX){
        strcat(test_name, "-m");
    }
    else{
        strcat(test_name, "-none");
    }
}

void free_all(){
    free(thread_IDs);
    free(list);
    for (int i = 0; i < n_list_elements; i++)
        free((void*) list_elements[i].key);
    free(list_elements);
    free(start_pos);
}

char generate_random_char(){
    return (char) rand() % 256;
}

void init_list(){
    // Initializes an empty list
    list = malloc(sizeof(SortedList_t));
    list->key = NULL;
    list->next = list;
    list->prev = list;

    // Creates and initializes (with random keys) the required number (threads x iterations) of list elements
    n_list_elements = n_threads * n_iterations;
    list_elements = malloc(sizeof(SortedListElement_t) * n_list_elements);
    srand(time(0));
    for (long long i = 0; i < n_list_elements; i++){
        char* key = malloc(sizeof(char));
        if (key == NULL) {
            free(list);
            free(list_elements);
            fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
            exit(1);
        }
        *key = generate_random_char();
        list_elements[i].key = key;
    }

    // Record starting position in list_elements for each thread
    start_pos = malloc(sizeof(long long) * n_threads);
    if (start_pos == NULL) {
        free(list);
        for (int i = 0; i < n_list_elements; i++)
            free((void*) list_elements[i].key);
        free(list_elements);
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (long long i = 0; i < n_threads; i++){
        start_pos[i] = i * n_iterations;
    }
}

void insert_delete(long long ins_or_del, long long i){
    if (ins_or_del == 0) {
        SortedList_insert(list, list_elements + i);
    }
    else {
        SortedListElement_t* curr = SortedList_lookup(list, list_elements[i].key);
        if (curr == NULL){
            fprintf(stderr, "Error: corrupted list\n");
            exit(2);
        }
        SortedList_delete(curr);
    }
}

void* insert_delete_all(void* start_pos){
    for (long long ins_or_del = 0; ins_or_del <= 1; ins_or_del++){
        for (long long i = *((long long*) start_pos); i < *((long long*) start_pos) + n_iterations; i++){
            switch(add_version){
                case NO_LOCK:
                    insert_delete(ins_or_del, i);
                    break;
                case MUTEX:
                    pthread_mutex_lock(&mutex);
                    insert_delete(ins_or_del, i);
                    pthread_mutex_unlock(&mutex);
                    break;
                case SPIN_LOCK:
                    while(__sync_lock_test_and_set(&spin_lock, 1));
                    insert_delete(ins_or_del, i);
                    __sync_lock_release(&spin_lock);
                    break;
                default:
                    break;
            }
        }
    }
    return NULL;
}

void sig_handler(int signum) {
    if (signum == SIGSEGV) {
        fprintf(stderr, "Error: Segmentation Fault\n");
        exit(2);
    }
}

int main(int argc, char **argv){
    // Process command line args
    process_args(argc, argv);
    process_name();

    signal(SIGSEGV, sig_handler);


    // Init list
    init_list();

    // Malloc space for thread pointers
    thread_IDs = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    if (thread_IDs == NULL) {
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    atexit(free_all);

    // Record start time
    struct timespec start_time;
    if (clock_gettime(CLOCK_MONOTONIC, &start_time) == -1){
        fprintf(stderr, "Error: clock_gettime failed\n%s\n", strerror(errno));
        exit(1);
    }

    // Starts the specified number of threads
    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&thread_IDs[i], NULL, insert_delete_all, start_pos + i) != 0) {
            fprintf(stderr, "Error: pthread_create failed\n");
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

    // Record end time
    struct timespec end_time;
    if (clock_gettime(CLOCK_MONOTONIC, &end_time) == -1){
        fprintf(stderr, "Error: clock_gettime failed\n%s\n", strerror(errno));
        exit(1);
    }

    if (SortedList_length(list) != 0){
        fprintf(stderr, "Error: corrupted list\n");
        exit(2);
    }

    // Calculate elapsed time
    long long elapsed_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000;
    elapsed_time_ns += end_time.tv_nsec;
    elapsed_time_ns -= start_time.tv_nsec;

    // Calculate stats
    long long n_operations = n_threads * n_iterations * 3;
    long long time_per_operation = elapsed_time_ns / n_operations;

    // Prints to stdout a comma-separated-value
    printf("%s,%d,%d,1,%lld,%lld,%lld\n", test_name, n_threads, n_iterations, n_operations, elapsed_time_ns, time_per_operation);

    // deal with freeing shit
    exit(0);
}