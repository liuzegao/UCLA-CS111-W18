#include <stdio.h>
#include <string.h>
#include "zlib.h"

#define BUFF_SIZE 1024

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

/*int main(){
    unsigned char deflated[BUFF_SIZE];
    unsigned char inflated[BUFF_SIZE];

    int num_bytes;

    num_bytes = read(0, deflated, BUFF_SIZE);
    num_bytes = inf(deflated, inflated);
    write(1, inflated, num_bytes);
}*/

int main(){
    int READ_SIZE = 1024;
    unsigned long size = 1024;
    unsigned char in[READ_SIZE];
    unsigned char out[READ_SIZE];

    int num_bytes = read(0, in, READ_SIZE);
    uncompress(out, &size, in, num_bytes);
    write(1, out, size);
}