#ifndef DECRYPT_H
#define DECRYPT_H

#include "chacha/chacha.h"

extern int decrypt_main(ChaCha *ctx, int optc, char *optv[]);

#endif