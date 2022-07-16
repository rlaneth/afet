#include "opts.h"
#include "input/input.h"
#include <stdlib.h>
#include <string.h>

#define IS_OPT(a, b, str) strcmp(a, str) == 0 || strcmp(b, str) == 0
#define NEXT_I()                                                               \
  i++;                                                                         \
  if (i >= optc) return OPTS_BAD_VALUE;

int parse_opts(Opts *out, int optc, char *optv[]) {
  Opts opts;
  char *curr;

  opts.infile = NULL;
  opts.outfile = NULL;
  opts.limit = 0;

  for (int i = 0; i < optc; i++) {
    curr = optv[i];

    if (IS_OPT("-i", "--input", curr)) {
      NEXT_I();
      opts.infile = optv[i];
      continue;
    }

    if (IS_OPT("-o", "--output", curr)) {
      NEXT_I();
      opts.outfile = optv[i];
      continue;
    }

    if (IS_OPT("-l", "--limit", curr)) {
      NEXT_I();
      if (read_uint64(&opts.limit, optv[i])) return OPTS_BAD_VALUE;
      continue;
    }

    return OPTS_BAD_NAME;
  }

  memcpy(out, &opts, sizeof(Opts));
  return OPTS_SUCCESS;
}