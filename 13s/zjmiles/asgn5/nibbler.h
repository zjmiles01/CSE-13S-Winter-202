#ifndef NIBBLER_H
#define NIBBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct s_nib {
    FILE *underlying_f;
    bool opened_for_read;
    uint32_t num_nibbles;
    uint8_t stored_nibble;
    bool has_half;
} NIB;

NIB *nib_open(const char *filename, const char *mode);
void nib_close(NIB *nib);
int nib_get_nibble(NIB *nib);
void nib_put_nibble(int nibble, NIB *nib);

#endif
