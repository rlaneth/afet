#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

extern int read_key(uint8_t *out, char *filename);
extern int read_uint64(uint64_t *out, char *in);

#endif