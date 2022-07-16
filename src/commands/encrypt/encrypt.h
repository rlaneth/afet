#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "chacha/chacha.h"

extern int encrypt_main(ChaCha *ctx, int optc, char *optv[]);

#endif