#include "encrypt.h"
#include "commands/commands.h"
#include "opts/opts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encrypt_main(ChaCha *ctx, int optc, char *optv[]) {
  int ret = COMMAND_SUCCESS;
  FILE *infile;
  FILE *outfile;
  size_t read;

  Opts *opts = malloc(sizeof(Opts));
  if (!opts) return COMMAND_MALLOC_FAILURE;

  uint8_t *message = malloc(CHACHA_BLOCK_SIZE);
  if (!message) {
    ret = COMMAND_MALLOC_FAILURE;
    goto cl_after_opts;
  }

  uint8_t *ciphertext = malloc(CHACHA_BLOCK_SIZE);
  if (!ciphertext) {
    ret = COMMAND_MALLOC_FAILURE;
    goto cl_after_message;
  }

  int err = parse_opts(opts, optc, optv);
  if (err || opts->limit || !opts->infile) {
    ret = COMMAND_BAD_OPTIONS;
    goto cl_after_ciphertext;
  }

  if (opts->outfile == NULL) {
    outfile = stdout;
  } else {
    outfile = fopen(opts->outfile, "w");
    if (outfile == NULL) {
      ret = COMMAND_FILE_READ_FAILURE;
      goto cl_after_ciphertext;
    }
  }

  infile = fopen(opts->infile, "r");
  if (infile == NULL) {
    ret = COMMAND_FILE_READ_FAILURE;
    goto cl_after_outfile;
  }

  while ((read = fread(message, 1, CHACHA_BLOCK_SIZE, infile)) > 0) {
    ChaCha_encrypt(ctx, ciphertext, message, read);
    fwrite(ciphertext, read, 1, outfile);
  }

  fclose(infile);

cl_after_outfile:
  if (opts->outfile != NULL) fclose(outfile);

cl_after_ciphertext:
  free(ciphertext);

cl_after_message:
  free(message);

cl_after_opts:
  free(opts);

  return ret;
}