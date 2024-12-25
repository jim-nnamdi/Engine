#include "safe.hh"
#include <string.h>

int encrypt_password(const char* password, unsigned char* ciphertext, unsigned char* nonce, unsigned char* key) {
    randombytes_buf(key,crypto_secretbox_KEYBYTES);
    randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
    int encrypt = crypto_secretbox_easy(ciphertext, (unsigned char*) password, strlen(password) + 1, nonce, key);
    return encrypt;
}

int decrypt_password(const unsigned char* ciphertext, unsigned char* decrypted_password, size_t ciphertext_len, unsigned char* nonce, unsigned char* key){
    if (crypto_secretbox_open_easy(decrypted_password,ciphertext,ciphertext_len,nonce, key) != 0) return 1;
    return 0;
}