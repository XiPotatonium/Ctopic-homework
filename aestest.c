#include "aes256.h"
#include "stdio.h"

#define DUMP(s, i, buf, sz)                              \
    {                                                    \
        printf(s);                                       \
        for (i = 0; i < (sz); i++) printf("%c ", buf[i]); \
        printf("\n");                                    \
    }

int main() {
    char key[32] = {'C', '_', 'T', 'O', 'P', 'I', 'C', 0};
    aes256_context_t aes;
    aes256_init(&aes, key);
    char data[16] = "TEST DATA";
    aes256_encrypt_ecb(&aes, data);
    int i;
    aes256_decrypt_ecb(&aes, data);
    printf("%s\n", data);
    return 0;
}