#ifndef _AES_256_H
#define _AES_256_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    uint8_t key[32];
    uint8_t enckey[32];
    uint8_t deckey[32];
} aes256_context_t;

uint8_t* aes256_get_key(char*);
aes256_context_t* aes256_init(uint8_t*);
void aes256_done(aes256_context_t*);

void aes256_encrypt_ecb(aes256_context_t*, uint8_t*);
void aes256_decrypt_ecb(aes256_context_t*, uint8_t*);

void aes256_encrypt(aes256_context_t*, uint8_t*, size_t size);
void aes256_decrypt(aes256_context_t*, uint8_t*, size_t size);

#endif