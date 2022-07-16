#include "chacha.h"
#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CHACHA_INTERNAL_ROTL(a, b, size) (a << b) | (a >> (size - b))
#define CHACHA_INTERNAL_QUARTERROUND(s, a, b, c, d)                            \
  s[a] += s[b];                                                                \
  s[d] ^= s[a];                                                                \
  s[d] = CHACHA_INTERNAL_ROTL(s[d], 16, 32);                                   \
  s[c] += s[d];                                                                \
  s[b] ^= s[c];                                                                \
  s[b] = CHACHA_INTERNAL_ROTL(s[b], 12, 32);                                   \
  s[a] += s[b];                                                                \
  s[d] ^= s[a];                                                                \
  s[d] = CHACHA_INTERNAL_ROTL(s[d], 8, 32);                                    \
  s[c] += s[d];                                                                \
  s[b] ^= s[c];                                                                \
  s[b] = CHACHA_INTERNAL_ROTL(s[b], 7, 32);

void ChaCha_reset(ChaCha *ctx, uint64_t counter, uint64_t nonce) {
  ctx->counter = counter;
  ctx->nonce = nonce;
}

void ChaCha_init(ChaCha *ctx, uint8_t rounds, uint8_t *key, uint64_t counter,
                 uint64_t nonce) {

  ctx->rounds = rounds;
  memcpy(ctx->key, key, 32);
  ChaCha_reset(ctx, counter, nonce);
}

void ChaCha_random(ChaCha *ctx, uint8_t *out) {
  uint32_t src[16];
  uint32_t dst[16];
  uint8_t i;

  src[0] = 0x61707865;
  src[1] = 0x3320646e;
  src[2] = 0x79622d32;
  src[3] = 0x6b206574;

  for (i = 0; i < 8; i++) {
    uint8_t base = i * 4;
    src[4 + i] = (ctx->key[base + 0]) | (ctx->key[base + 1] << 8) |
                 (ctx->key[base + 2] << 16) | (ctx->key[base + 3] << 24);
  }

  src[12] = ctx->counter & 0xffffffff;
  src[13] = ctx->counter >> 32;
  src[14] = ctx->nonce & 0xffffffff;
  src[15] = ctx->nonce >> 32;

  memcpy(&dst, &src, 64);

  for (i = 0; i < ctx->rounds; i += 2) {
    CHACHA_INTERNAL_QUARTERROUND(dst, 0, 4, 8, 12);
    CHACHA_INTERNAL_QUARTERROUND(dst, 1, 5, 9, 13);
    CHACHA_INTERNAL_QUARTERROUND(dst, 2, 6, 10, 14);
    CHACHA_INTERNAL_QUARTERROUND(dst, 3, 7, 11, 15);
    CHACHA_INTERNAL_QUARTERROUND(dst, 0, 5, 10, 15);
    CHACHA_INTERNAL_QUARTERROUND(dst, 1, 6, 11, 12);
    CHACHA_INTERNAL_QUARTERROUND(dst, 2, 7, 8, 13);
    CHACHA_INTERNAL_QUARTERROUND(dst, 3, 4, 9, 14);
  }

  for (i = 0; i < 16; i++)
    src[i] = htole32(dst[i] + src[i]);
  memcpy(out, src, 64);

  ctx->counter++;
}

void ChaCha_encrypt(ChaCha *ctx, uint8_t *out, uint8_t *message,
                    uint64_t message_size) {
  uint8_t blk[64];
  uint64_t pos;

  for (uint64_t i = 0; i < message_size; i++) {
    pos = i % 64;
    if (pos == 0) ChaCha_random(ctx, blk);
    out[i] = message[i] ^ blk[pos];
  }
}

void ChaCha_decrypt(ChaCha *ctx, uint8_t *out, uint8_t *encrypted,
                    uint64_t encrypted_size) {
  return ChaCha_encrypt(ctx, out, encrypted, encrypted_size);
}