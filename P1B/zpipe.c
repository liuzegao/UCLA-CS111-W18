#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#define BUFF_SIZE 1024

int def(unsigned char in[BUFF_SIZE], int num_bytes_in, unsigned char out[BUFF_SIZE]){
    int ret, flush;
    unsigned have;
    z_stream stdin_to_shell;
    //unsigned char in[BUFF_SIZE];
    //unsigned char out[BUFF_SIZE];

    stdin_to_shell.zalloc = Z_NULL;
    stdin_to_shell.zfree = Z_NULL;
    stdin_to_shell.opaque = Z_NULL;

    ret = deflateInit(&stdin_to_shell, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        return ret;

    stdin_to_shell.avail_in = num_bytes_in;
    stdin_to_shell.next_in = in;
    stdin_to_shell.avail_out = BUFF_SIZE;
    stdin_to_shell.next_out = out;
    do {
        deflate(&stdin_to_shell, Z_SYNC_FLUSH);
    } while(stdin_to_shell.avail_in > 0);
 
    deflateEnd(&stdin_to_shell);
    return ret;
}

int inf(unsigned char in[BUFF_SIZE], int num_bytes_in, unsigned char out[BUFF_SIZE]) {
    int ret;
    unsigned have;
    z_stream shell_to_stdout;
    //unsigned char in[BUFF_SIZE];
    //unsigned char out[BUFF_SIZE];

    shell_to_stdout.zalloc = Z_NULL;
    shell_to_stdout.zfree = Z_NULL;
    shell_to_stdout.opaque = Z_NULL;

    ret = inflateInit(&shell_to_stdout);
    if (ret != Z_OK)
        return ret;

    shell_to_stdout.avail_in = num_bytes_in;
    shell_to_stdout.next_in = in;
    shell_to_stdout.avail_out = BUFF_SIZE;
    shell_to_stdout.next_out = out;
    do {
        inflate(&shell_to_stdout, Z_SYNC_FLUSH);
    } while(shell_to_stdout.avail_in > 0);

    inflateEnd(&shell_to_stdout);
    return ret;
}

int main(int argc, char **argv){
    int ret;
    unsigned char in[BUFF_SIZE] = "fa";
    unsigned char out[BUFF_SIZE];

    def(in, 10, out);
    write(1, out, 3);
    write(1, "\n", 1);

    inf(out, 10, in);
    write(1, in, 3);
    write(1, "\n", 1);

}