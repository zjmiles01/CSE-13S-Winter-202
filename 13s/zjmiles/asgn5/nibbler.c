#include <stdlib.h>
#include "nibbler.h"

NIB *nib_open(const char *filename, const char *mode) {
    if (!filename || !mode) return NULL;

    NIB *nib = malloc(sizeof(NIB));
    if (!nib) return NULL;

    nib->underlying_f = fopen(filename, mode);
    if (!nib->underlying_f) {
        free(nib);
        return NULL;
    }

    nib->opened_for_read = (mode[0] == 'r');
    nib->num_nibbles = 0;
    nib->stored_nibble = 0;
    nib->has_half = false;

    return nib;
}

int nib_get_nibble(NIB *nib) {
    if (!nib || !nib->opened_for_read) return EOF;

    int result;

    if (nib->has_half) {
        nib->has_half = false;
        result = nib->stored_nibble;
    } else {
        int byte = fgetc(nib->underlying_f);
        if (byte == EOF) return EOF;

        result = (byte >> 4) & 0x0F;
        nib->stored_nibble = byte & 0x0F;
        nib->has_half = true;
    }

    nib->num_nibbles++;   // 🔥 IMPORTANT
    return result;
}

void nib_put_nibble(int nibble, NIB *nib) {
    if (!nib || nib->opened_for_read) return;

    nibble &= 0x0F;

    if (!nib->has_half) {
        nib->stored_nibble = nibble << 4;
        nib->has_half = true;
    } else {
        unsigned char byte = nib->stored_nibble | nibble;
        fputc(byte, nib->underlying_f);
        nib->has_half = false;
    }

    nib->num_nibbles++;   // 🔥 IMPORTANT
}

void nib_close(NIB *nib) {
    if (!nib) return;

    if (!nib->opened_for_read && nib->has_half) {
        fputc(nib->stored_nibble, nib->underlying_f);
    }

    if (nib->underlying_f) {
        fclose(nib->underlying_f);
        nib->underlying_f = NULL;
    }

    free(nib);
}
