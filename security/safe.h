#include <sodium.h>

int encrypt_password(const char *password, unsigned char *ciphertext, unsigned char *nonce, unsigned char *key);
int decrypt_password(const unsigned char *ciphertext, unsigned char *decrypted_password, size_t ciphertext_len, unsigned char *nonce, unsigned char *key);
