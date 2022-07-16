#ifndef COMMANDS_H
#define COMMANDS_H

#include "chacha/chacha.h"
#include "decrypt/decrypt.h"
#include "encrypt/encrypt.h"
#include "random/random.h"
#include <stdint.h>

#define COMMAND_SUCCESS 0
#define COMMAND_FAILURE 1
#define COMMAND_BAD_OPTIONS 2
#define COMMAND_MALLOC_FAILURE 3
#define COMMAND_FILE_READ_FAILURE 4

typedef int (*Entrypoint)(ChaCha *ctx, int optc, char *optv[]);

struct CommandSpec {
  const char *name;
  const Entrypoint entrypoint;
};

extern Entrypoint commands_get_entrypoint(char *name);

#endif