#include "chacha/chacha.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_key(uint8_t *out, char *filename) {
  uint8_t buf[CHACHA_KEY_SIZE];

  FILE *file = fopen(filename, "r");
  if (file == NULL) return 1;

  int read = fread(buf, CHACHA_KEY_SIZE, 1, file);
  if (read != 1) {
    fclose(file);
    return 2;
  }

  fseek(file, 0, SEEK_END);
  if (ftell(file) != CHACHA_KEY_SIZE) {
    fclose(file);
    return 3;
  }

  memcpy(out, buf, CHACHA_KEY_SIZE);
  fclose(file);

  return 0;
}

int read_uint64(uint64_t *out, char *in) {
  int preverr = errno;
  int ret = 0;
  errno = 0;

  char *end = NULL;
  unsigned long long value = strtoull(in, &end, 10);
  if (errno || *end || in == end || value > UINT64_MAX) {
    ret = 1;
  } else {
    *out = (uint64_t)value;
  }

  errno = preverr;
  return ret;
}