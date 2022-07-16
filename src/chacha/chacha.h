#ifndef CHACHA_H
#define CHACHA_H

#include <stddef.h>
#include <stdint.h>

#define CHACHA_BLOCK_SIZE 64
#define CHACHA_KEY_SIZE 32

typedef struct {
  uint8_t key[32];
  uint8_t rounds;
  uint64_t counter;
  uint64_t nonce;
} ChaCha;

extern void ChaCha_reset(ChaCha *ctx, uint64_t counter, uint64_t nonce);

extern void ChaCha_init(ChaCha *ctx, uint8_t rounds, uint8_t *key,
                        uint64_t counter, uint64_t nonce);

extern void ChaCha_random(ChaCha *ctx, uint8_t *out);

extern void ChaCha_encrypt(ChaCha *ctx, uint8_t *out, uint8_t *message,
                           uint64_t message_size);

extern void ChaCha_decrypt(ChaCha *ctx, uint8_t *out, uint8_t *encrypted,
                           uint64_t encrypted_size);

#endif