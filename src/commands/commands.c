#include <stdio.h>
#include <string.h>

#include "commands.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

struct CommandSpec commands[] = {{"random", random_main},
                                 {"encrypt", encrypt_main},
                                 {"decrypt", decrypt_main}};

Entrypoint commands_get_entrypoint(char *name) {
  for (size_t i = 0; i < ARRAY_SIZE(commands); i++) {
    if (strcmp(name, commands[i].name) == 0) {
      return commands[i].entrypoint;
    }
  }
  return NULL;
}