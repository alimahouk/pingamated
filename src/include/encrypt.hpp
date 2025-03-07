//
//  encrypt.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <iostream>
#include <openssl/ssl.h>

namespace pingamate {
        namespace crypto {
                int aes_decrypt(const unsigned char *ciphertext, const int ciphertext_len, const unsigned char *key, unsigned char **plaintext);
                int aes_encrypt(const unsigned char *plaintext, const int plaintext_len, const unsigned char *key, unsigned char **ciphertext);
                int ecdh(EVP_PKEY *private_key, EVP_PKEY *public_key, unsigned char **secret);
        }
}

#endif /* ENCRYPT_HPP */
