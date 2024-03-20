#pragma once
#include <stdint.h>

typedef uint8_t aes_block_t[16u];
typedef uint8_t aes_key_t[16u];

void aes_encrypt_block(aes_block_t plain, const aes_key_t key);
void aes_decrypt_block(aes_block_t cipher, const aes_key_t key);

// Compute the initial add_round_key and s_box lookup
void aes_first_round_s_box(aes_block_t plain, const aes_key_t key);
