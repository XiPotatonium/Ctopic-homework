#ifndef _AES_256_H
#define _AES_256_H

#include <stdlib.h>
#include <string.h>

#ifndef uint8_t
#define uint8_t unsigned char
#endif

typedef struct {
    uint8_t key[32];
    uint8_t enckey[32];
    uint8_t deckey[32];
} aes256_context_t;

typedef struct {
    void* ptr;
    int encrypted;
    size_t size;
} aes256_data_t;

uint8_t* aes256_get_key(char*);
void aes256_init(aes256_context_t*, uint8_t*);
void aes256_done(aes256_context_t*);

void aes256_encrypt_ecb(aes256_context_t*, uint8_t*);
void aes256_decrypt_ecb(aes256_context_t*, uint8_t*);

void aes256_encrypt(aes256_context_t* aes, aes256_data_t* data);
void aes256_decrypt(aes256_context_t* aes, aes256_data_t* data);
aes256_data_t* aes256_data_new(void* ptr, size_t size);
void* aes256_data_get(aes256_data_t* aes);
void aes256_data_free(aes256_data_t* data);

#endif