#ifndef RANDOM_H
#define RANDOM_H

#include "chacha/chacha.h"

extern int random_main(ChaCha *ctx, int optc, char *optv[]);

#endif