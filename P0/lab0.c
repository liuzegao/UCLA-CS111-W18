// NAME: Quentin Truong
// EMAIL: quentintruong@gmail.com
// ID: 404782322

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <fcntl.h> // open
#include <string.h>
#include <unistd.h> // close, dup, read

/*
 If you are unable to open the specified input file,
 report the failure (on stderr, file descriptor 2) using fprintf(3),
 and exit(2) with a return code of 2.
 */
void open_infile(char* in_file){
    int fd0 = open(in_file, O_RDONLY);
    if(fd0 >= 0)
    {
        close(0);
        dup(fd0);
        close(fd0);
    }
    else
    {
        fprintf(stderr, "Cannot open input file '%s'\n", in_file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(2);
    }
}

/*
 If you are unable to create the specified output file,
 report the failure (on stderr, file descriptor 2) using fprintf(3),
 and exit(2) with a return code of 3.
 */
void create_outfile(char* out_file){
    int fd1 = creat(out_file, S_IRWXU);
    if (fd1 >= 0) {
        close(1);
        dup(fd1);
        close(fd1);
    }
    else{
        fprintf(stderr, "Cannot open output file '%s'\n", out_file);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(3);
    }
}

/*
 handler that catches the segmentation fault,
 logs an error message (on stderr, file descriptor 2)
 and exit(2) with a return code of 4.
 */
void handler(int num) {
    fprintf(stderr, "Segmentation fault, signal number %d\n", num);
    exit(4);
}

/*
 subroutine that sets a char * pointer to NULL
 and then stores through the null pointer
 */
void force_segfault(void){
    char* n = NULL;
    *n = '0';
}

void copy_stdin_stdout(void){
    char* buffer;
    buffer = malloc(sizeof(char));
    while(read(0, buffer, 1) > 0)
        write(1, buffer, 1);
    free(buffer);
}

int main (int argc, char **argv){
    int opt;
    static int segfault = 0;
    static int catch = 0;
    char* in_file = NULL;
    char* out_file = NULL;
    static struct option long_options[] = {
        {"segfault", no_argument, &segfault, 1}, // flag
        {"catch", no_argument, &catch, 1}, // flag
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "i:o:", long_options, NULL)) != -1){
        switch (opt){
            case 0:
                /* Flag */
                break;
                
            case 'i':
                in_file = optarg;
                break;
                
            case 'o':
                out_file = optarg;
                break;
                
            case '?':
                /* Missing required arg */
                exit(1);
                break;
                
            default:
                fprintf(stderr, "Unrecognized argument. See correct usage below\n./lab0 --segfault\n");
                exit(1);
        }
    }
    if (in_file)
        open_infile(in_file);
    if (out_file)
        create_outfile(out_file);
    if (catch)
        signal(SIGSEGV, handler);
    if (segfault)
        force_segfault();
    
    copy_stdin_stdout();
    
    exit(0);
}
