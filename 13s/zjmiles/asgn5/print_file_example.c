#include <stdio.h>
#include <stdlib.h>

void print_file_example(const char *filename) {
    FILE *f = fopen(filename, "r");

    if (f == NULL) {
        printf("Can't open file for input: %s\n", filename);
        exit(1);
    }

    while (1) {
        int ch = fgetc(f);
        if (ch == EOF) break;

        // Print ch, but here the program could write to an output file.
        printf("%c", ch);
    }

    fclose(f);
}

int main(void) {
    print_file_example("jabberwocky.txt");
    return 0;
}
