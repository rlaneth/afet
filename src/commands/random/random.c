#include "random.h"
#include "chacha/chacha.h"
#include "commands/commands.h"
#include "input/input.h"
#include "opts/opts.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int random_main(ChaCha *ctx, int optc, char *optv[]) {
  int ret = COMMAND_SUCCESS;
  FILE *outfile;
  uint64_t i;

  Opts *opts = malloc(sizeof(Opts));
  if (!opts) return COMMAND_MALLOC_FAILURE;

  uint8_t *block = malloc(CHACHA_BLOCK_SIZE);
  if (!block) {
    ret = COMMAND_MALLOC_FAILURE;
    goto cl_after_opts;
  }

  int err = parse_opts(opts, optc, optv);
  if (err || opts->infile) {
    ret = COMMAND_BAD_OPTIONS;
    goto cl_after_block;
  }

  if (!opts->limit) opts->limit = UINT64_MAX;

  if (!opts->outfile) {
    outfile = stdout;
  } else {
    outfile = fopen(opts->outfile, "w");
    if (!outfile) {
      ret = COMMAND_FILE_READ_FAILURE;
      goto cl_after_block;
    }
  }

  uint64_t max_blocks = (opts->limit / CHACHA_BLOCK_SIZE);
  uint64_t bytes_remainder = (opts->limit % CHACHA_BLOCK_SIZE);

  for (i = 0; i < max_blocks; i++) {
    ChaCha_random(ctx, block);
    fwrite(block, CHACHA_BLOCK_SIZE, 1, outfile);
  }

  ChaCha_random(ctx, block);
  fwrite(block, bytes_remainder, 1, outfile);

  if (opts->outfile != NULL) fclose(outfile);

cl_after_block:
  free(block);

cl_after_opts:
  free(opts);

  return ret;
}