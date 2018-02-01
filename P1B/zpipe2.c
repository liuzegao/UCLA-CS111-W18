/* zpipe.c: example of proper use of zlib's inflate() and deflate()
   Not copyrighted -- provided to the public domain
   Version 1.4  11 December 2005  Mark Adler */

/* Version history:
   1.0  30 Oct 2004  First version
   1.1   8 Nov 2004  Add void casting for unused return values
                     Use switch statement for inflate() return values
   1.2   9 Nov 2004  Add assertions to document zlib guarantees
   1.3   6 Apr 2005  Remove incorrect assertion in inf()
   1.4  11 Dec 2005  Add hack to avoid MSDOS end-of-line conversions
                     Avoid some compiler warnings for input and output buffers
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"


/* compress or decompress from stdin to stdout */
int main(int argc, char **argv){
    const int READ_SIZE = 1024;
    unsigned char in[READ_SIZE];
    unsigned char buffer[READ_SIZE];
    unsigned char out[READ_SIZE]

    int num_bytes = read(0, in, READ_SIZE);
    compress(buffer, READ_SIZE, in, num_bytes);
    compress(out, READ_SIZE, in, num_bytes);
}