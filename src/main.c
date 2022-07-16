#include "chacha/chacha.h"
#include "commands/commands.h"
#include "input/input.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME "afet"
#define PRINTF_E(message, ...)                                                 \
  fprintf(stderr, PROGRAM_NAME ": " message "\n", __VA_ARGS__);
#define PRINT_E(message) fprintf(stderr, PROGRAM_NAME ": " message "\n");

void usage() {
  printf("usage: %s COMMAND [OPTIONS]... FILENAME NONCE\n", PROGRAM_NAME);
}

int main(int argc, char *argv[]) {
  int ret = EXIT_SUCCESS;

  if (argc < 2) {
    usage();
    return EXIT_FAILURE;
  }

  char *command = argv[1];
  Entrypoint entrypoint = commands_get_entrypoint(command);
  if (entrypoint == NULL) {
    PRINTF_E("'%s' is not a valid command.", command);
    return EXIT_FAILURE;
  }

  if (argc < 4) {
    usage();
    return EXIT_FAILURE;
  }

  uint8_t key[CHACHA_KEY_SIZE];
  uint64_t nonce;

  char *filename = argv[argc - 2];
  char *nstr = argv[argc - 1];

  int err = read_key(key, filename);
  switch (err) {
  case 0:
    break;

  case 2:
    PRINTF_E("file '%s' is shorter than %d bytes.", filename, CHACHA_KEY_SIZE);
    return EXIT_FAILURE;

  case 3:
    PRINTF_E("file '%s' is longer than %d bytes.", filename, CHACHA_KEY_SIZE);
    return EXIT_FAILURE;

  default:
    PRINTF_E("unable to open file '%s'.", filename);
    return EXIT_FAILURE;
  }

  err = read_uint64(&nonce, nstr);
  if (err) {
    PRINTF_E("invalid nonce value '%s'.", nstr);
    return EXIT_FAILURE;
  }

  ChaCha *ctx = malloc(sizeof(ChaCha));
  if (!ctx) {
    PRINT_E("failed to allocate memory.");
    return EXIT_FAILURE;
  }

  ChaCha_init(ctx, 20, key, 1, nonce);

  int optc = argc - 4;
  char **optv = argv + 2;
  int cmd_ret = entrypoint(ctx, optc, optv);

  if (cmd_ret != COMMAND_SUCCESS) {
    ret = EXIT_FAILURE;
    switch (cmd_ret) {
    case COMMAND_BAD_OPTIONS:
      PRINTF_E("bad command line options for '%s'.", command);
      break;
    case COMMAND_MALLOC_FAILURE:
      PRINT_E("failed to allocate memory.");
      break;
    default:
      PRINTF_E("command '%s' failed.", command);
    }
  }

  free(ctx);
  return ret;
}