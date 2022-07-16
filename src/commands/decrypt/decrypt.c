#include "decrypt.h"
#include "commands/encrypt/encrypt.h"
#include <stdlib.h>

int decrypt_main(ChaCha *ctx, int optc, char *optv[]) {
  return encrypt_main(ctx, optc, optv);
}