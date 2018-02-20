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
int n_lists = 1;            // number of sub-lists
char lock_version = NO_LOCK; // 

// Globals
char test_name[20] = "list";
long long n_list_elements = 0;

pthread_t* thread_IDs = NULL;
SortedList_t* list_heads = NULL;
SortedListElement_t* list_elements = NULL;
long long* thread_nums = NULL;   // numbering threads [0, n_threads-1]
pthread_mutex_t* list_mutexes = NULL;
int* list_spin_lock = NULL;
long long* thread_lock_wait_time = NULL;

// Get-opt Long Options parsing
void process_args(int argc, char **argv){
    int opt;
    static struct option long_options[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"lists", required_argument, 0, 'l'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "t:i:l:y:s:", long_options, NULL)) != -1){
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
            case 'l':
                n_lists = atoi(optarg);
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
                        lock_version = MUTEX;
                        break;
                    case SPIN_LOCK:
                        lock_version = SPIN_LOCK;
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

    if (lock_version == SPIN_LOCK){
        strcat(test_name, "-s");
    }
    else if (lock_version == MUTEX){
        strcat(test_name, "-m");
    }
    else{
        strcat(test_name, "-none");
    }
}

void free_all(){
    if (thread_IDs != NULL)
        free(thread_IDs);
    if (list_heads != NULL)
        free(list_heads);
    for (int i = 0; i < n_list_elements; i++)
        free((void*) list_elements[i].key);
    if (list_elements != NULL)
        free(list_elements);
    if (thread_nums != NULL)
        free(thread_nums);
    if (list_mutexes != NULL)
        free(list_mutexes);
    if (list_spin_lock != NULL)
        free(list_spin_lock);
    if (thread_lock_wait_time != NULL)
        free(thread_lock_wait_time);
}

char generate_random_char(){
    return 'a' + (char) rand() % 26;
}

void get_time(struct timespec* time){
    if (clock_gettime(CLOCK_MONOTONIC, time) == -1){
        fprintf(stderr, "Error: clock_gettime failed\n%s\n", strerror(errno));
        exit(1);
    }
}

long long calculate_elapsed_ns(struct timespec* start_time, struct timespec* end_time){
    long long elapsed_time_ns = (end_time->tv_sec - start_time->tv_sec) * 1000000000;
    elapsed_time_ns += end_time->tv_nsec;
    elapsed_time_ns -= start_time->tv_nsec;
    return elapsed_time_ns;
}

void init_list(){
    // Malloc and initializes an empty list
    list_heads = malloc(sizeof(SortedList_t) * n_lists);
    if (list_heads == NULL) {
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (int i = 0; i < n_lists; i++){
        (list_heads + i)->key = NULL;
        (list_heads + i)->next = list_heads + i;
        (list_heads + i)->prev = list_heads + i;
    }

    // Malloc and initialize mutexes for all sublists 
    list_mutexes = malloc(sizeof(pthread_mutex_t) * n_lists);
    if (list_mutexes == NULL){
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (int i = 0; i < n_lists; i++)
        pthread_mutex_init(list_mutexes + i, NULL);

    // Malloc and initialize spinlocks for all sublists
    list_spin_lock = malloc(sizeof(int) * n_lists); // todo, malloc only one or the other (mutex vs. spinlock)
    if (list_spin_lock == NULL){
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (int i = 0; i < n_lists; i++)
        list_spin_lock[i] = 0;

    // Malloc and initialize all per-thread total time to obtain lock
    thread_lock_wait_time = malloc(sizeof(long long)* n_threads);
    if (thread_lock_wait_time == NULL){
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (int i = 0; i < n_threads; i++)
        thread_lock_wait_time[i] = 0;

    // Malloc and initializes (with random keys) the required number (threads x iterations) of list elements
    n_list_elements = n_threads * n_iterations;
    list_elements = malloc(sizeof(SortedListElement_t) * n_list_elements);
    if (list_elements == NULL){
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    srand(time(0));
    for (long long i = 0; i < n_list_elements; i++){
        char* key = malloc(sizeof(char));
        if (key == NULL){
            fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
            exit(1);
        }
        *key = generate_random_char();
        list_elements[i].key = key;
    }

    // Malloc and record starting position in list_elements for each thread
    thread_nums = malloc(sizeof(long long) * n_threads);
    if (thread_nums == NULL){
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }
    for (long long i = 0; i < n_threads; i++)
        thread_nums[i] = i;
}

int simple_hash(const char* key){
    return (int) (*key);
}

void insert(long long i, int sublist_num){
    SortedList_insert(list_heads + sublist_num, list_elements + i);
}

void length(int sublist_num){
    if (SortedList_length(list_heads + sublist_num) == -1){
        fprintf(stderr, "Error: corrupted list\n");
        exit(2);
    }
}

void delete(long long i, int sublist_num){
    SortedListElement_t* curr = SortedList_lookup(list_heads + sublist_num, list_elements[i].key);
    if (curr == NULL){
        fprintf(stderr, "Error: corrupted list\n");
        exit(2);
    }
    if (SortedList_delete(curr) == 1){
        fprintf(stderr, "Error: corrupted list\n");
        exit(2);
    }
}

void* insert_length_delete(void* thread_num){
    long long start_pos = *((long long*) thread_num) * n_iterations;
    struct timespec start_time;
    struct timespec end_time;
    long long elapsed_time_ns;

    // Insert
    for (long long i = start_pos; i < start_pos + n_iterations; i++){        
        int sublist_num = simple_hash((list_elements + i)->key) % n_lists; // index choosing sublist from list_heads  

        switch(lock_version){
            case NO_LOCK:
                insert(i, sublist_num);
                break;
            case MUTEX:
                // Record start time and try lock
                get_time(&start_time);
                pthread_mutex_lock(list_mutexes + sublist_num);

                // Record end time and calculate elapsed time
                get_time(&end_time);
                elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);
                thread_lock_wait_time[*(int*) thread_num] += elapsed_time_ns;

                insert(i, sublist_num);
                pthread_mutex_unlock(list_mutexes + sublist_num);
                break;
            case SPIN_LOCK:
                // Record start time and try lock
                get_time(&start_time);
                while(__sync_lock_test_and_set(list_spin_lock + sublist_num, 1));

                // Record end time and calculate elapsed time
                get_time(&end_time);
                elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);
                thread_lock_wait_time[*(int*) thread_num] += elapsed_time_ns;

                insert(i, sublist_num);
                __sync_lock_release(list_spin_lock + sublist_num);
                break;
            default:
                break;
        }
    }

    // Length
    // Record start time and try lock
    get_time(&start_time);
    for (long long i = 0; i < n_lists; i++){
        switch(lock_version){
            case NO_LOCK:
                break;
            case MUTEX:
                pthread_mutex_lock(list_mutexes + i);
                break;
            case SPIN_LOCK:
                while(__sync_lock_test_and_set(list_spin_lock + i, 1));
                break;
            default:
                break;
        }
    }
    // Record end time and calculate elapsed time
    get_time(&end_time);
    elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);
    thread_lock_wait_time[*(int*) thread_num] += elapsed_time_ns;

    for (long long i = 0; i < n_lists; i++)
        length(i);
    for (long long i = 0; i < n_lists; i++){
        switch(lock_version){
            case NO_LOCK:
                break;
            case MUTEX:
                pthread_mutex_unlock(list_mutexes + i);
                break;
            case SPIN_LOCK:
                __sync_lock_release(list_spin_lock + i);
                break;
            default:
                break;
        }
    }

    // Delete
    for (long long i = start_pos; i < start_pos + n_iterations; i++){        
        int sublist_num = simple_hash((list_elements + i)->key) % n_lists; // index choosing sublist from list_heads  

        switch(lock_version){
            case NO_LOCK:
                delete(i, sublist_num);
                break;
            case MUTEX:
                // Record start time and try lock
                get_time(&start_time);
                pthread_mutex_lock(list_mutexes + sublist_num);

                // Record end time and calculate elapsed time
                get_time(&end_time);
                elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);
                thread_lock_wait_time[*(int*) thread_num] += elapsed_time_ns;

                delete(i, sublist_num);
                pthread_mutex_unlock(list_mutexes + sublist_num);
                break;
            case SPIN_LOCK:
                // Record start time and try lock
                get_time(&start_time);
                while(__sync_lock_test_and_set(list_spin_lock + sublist_num, 1));

                // Record end time and calculate elapsed time
                get_time(&end_time);
                elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);
                thread_lock_wait_time[*(int*) thread_num] += elapsed_time_ns;

                delete(i, sublist_num);
                __sync_lock_release(list_spin_lock + sublist_num);
                break;
            default:
                break;
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

    // Init list and prepare to free all mallocs
    atexit(free_all);
    init_list();

    // Malloc space for thread pointers
    thread_IDs = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    if (thread_IDs == NULL) {
        fprintf(stderr, "Error: malloc failed\n%s\n", strerror(errno));
        exit(1);
    }

    // Record start time
    struct timespec start_time;
    get_time(&start_time);

    // Starts the specified number of threads
    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&thread_IDs[i], NULL, insert_length_delete, thread_nums + i) != 0) {
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

    // Record end time and calculate elapsed time
    struct timespec end_time;
    get_time(&end_time);
    long long elapsed_time_ns = calculate_elapsed_ns(&start_time, &end_time);

    // Calculate stats
    long long n_operations = n_threads * n_iterations * 3;
    long long avg_time_per_operation = elapsed_time_ns / n_operations;

    // Calculate ...
    long long total_lock_wait_time_ns = 0;
    for (int i = 0; i < n_threads; i++)
        total_lock_wait_time_ns += thread_lock_wait_time[i];
    long long n_lock_operations = n_threads * (n_iterations * 2 + 1);
    long long avg_lock_wait_time_ns = total_lock_wait_time_ns / n_lock_operations;

    // Prints to stdout a comma-separated-value
    printf("%s,%d,%d,%d,%lld,%lld,%lld,%lld\n", test_name, n_threads, n_iterations, n_lists, n_operations, elapsed_time_ns, avg_time_per_operation, avg_lock_wait_time_ns);

    exit(0);
}