/*
* Program:      decompress
*
* Purpose:      Decompress a file that was compressed with "compress".
*
* Usage:        decompress INFILE -o OUTFILE
*/

#include "nibbler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void) {
    printf("Usage:  decompress INFILE -o OUTFILE\n");
    exit(0);
}

void decompress(NIB *nib, FILE *fout) {
}

int main(int argc, char **argv) {
    return 0;
}
