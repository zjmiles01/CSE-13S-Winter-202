#include <stdio.h>
#include <stdlib.h>

void write_file_example(const char *filename) {
    FILE *f = fopen(filename, "w");

    if (f == NULL) {
        printf("Can’t open file for output: %s\n", filename);
        exit(1);
    }

    char string[] = "Write this string to the file.\n";
    char *p = string;

    while (*p != '\0') {
        fputc(*p, f);
        ++p;
    }

    fclose(f);
}

int main(void) {
    write_file_example("out.txt");
    return 0;
}
