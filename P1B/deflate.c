#include <stdio.h>
#include <string.h>
#include "zlib.h"

#define BUFF_SIZE 1024

int def2(unsigned char in[BUFF_SIZE], int num)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[BUFF_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        return ret;

    strm.avail_in = num;
    strm.next_in = in;

    do {
        strm.avail_out = BUFF_SIZE;
        strm.next_out = out;
        ret = deflate(&strm, Z_SYNC_FLUSH); 
        have = BUFF_SIZE - strm.avail_out;
        write(1, out, have);
    } while(strm.avail_out == 0);

    (void) deflateEnd(&strm);
    return Z_OK;
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

    stdin_to_shell.avail_in = strlen((char*) in) + 1;
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

/*
int main(){
    unsigned char in[BUFF_SIZE] = "h";
    //read(0, in, 20);
    unsigned char deflated[BUFF_SIZE];

    int num_bytes;

    num_bytes = def2(in, 2);
    write(1, deflated, num_bytes);
}
*/

int main(){
    int READ_SIZE = 1024;
    unsigned long size = 1024;
    unsigned char in[READ_SIZE];
    unsigned char out[READ_SIZE];

    in[0] = 'h';
    in[1] = 'a';

    //int num_bytes = read(0, in, READ_SIZE);
    compress(out, &size, in + 1, 1);
    write(1, out, size);
}



