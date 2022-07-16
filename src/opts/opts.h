#ifndef OPTS_H
#define OPTS_H

#include <stdint.h>

#define OPTS_BAD_NAME 2
#define OPTS_BAD_VALUE 1
#define OPTS_SUCCESS 0

typedef struct {
  char *infile;
  char *outfile;
  uint64_t limit;
} Opts;

extern int parse_opts(Opts *out, int optc, char *optv[]);

#endif