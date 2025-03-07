//
//  encrypt.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "encrypt.hpp"

#include <openssl/conf.h>
#include <openssl/err.h>

#include "crypto.hpp"
#include "sha.hpp"

/**
 * AES algorithms courtesy of the
 * <a href="https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption">OpenSSL wiki</a>.
 * @return The length of the plaintext.
 */
int pingamate::crypto::aes_decrypt(const unsigned char *ciphertext, const int ciphertext_len, const unsigned char *key, unsigned char **plaintext)
{
        EVP_CIPHER_CTX *ctx;
        int len;
        
        *plaintext = NULL;
        ctx        = NULL;
        len        = 0;
        
        // Create & initialise the context.
        if ( !(ctx = EVP_CIPHER_CTX_new()) )
                error();
        
        /*
         * Initialise the decryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher.
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits.
         */
        if ( EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1 ) {
                error();
                
                return -1;
        }
        
        /*
         * Provide the message to be decrypted, and obtain the plaintext output.
         * EVP_DecryptUpdate can be called multiple times if necessary.
         */
        if ( EVP_DecryptUpdate(ctx, *plaintext, &len, ciphertext, ciphertext_len) != 1 ) {
                error();
                
                return -1;
        }
        
        /*
         * Finalise the decryption. Further plaintext bytes may be written at
         * this stage.
         */
        if ( EVP_DecryptFinal_ex(ctx, *plaintext + len, &len) != 1 ) {
                error();
                
                return -1;
        }
        
        // Clean up.
        EVP_CIPHER_CTX_free(ctx);
        
        return len;
}

/**
 * AES algorithms courtesy of the
 * <a href="https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption">OpenSSL wiki</a>.
 * @return The length of the cyphertext.
 */
int pingamate::crypto::aes_encrypt(const unsigned char *plaintext, const int plaintext_len, const unsigned char *key, unsigned char **ciphertext)
{
        EVP_CIPHER_CTX *ctx;
        int len;
        
        ctx = NULL;
        len = 0;
        
        // Create & initialise the context.
        if ( !(ctx = EVP_CIPHER_CTX_new()) )
                error();
        
        /*
         * Initialise the encryption operation. IMPORTANT - ensure you use a key
         * and IV size appropriate for your cipher.
         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
         * IV size for *most* modes is the same as the block size. For AES this
         * is 128 bits.
         */
        if ( EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1 )
                error();
        
        /*
         * Provide the message to be encrypted, and obtain the encrypted output.
         * EVP_EncryptUpdate can be called multiple times if necessary.
         */
        if ( EVP_EncryptUpdate(ctx, *ciphertext, &len, plaintext, plaintext_len) != 1 )
                error();
        
        /*
         * Finalise the encryption. Further ciphertext bytes may be written at
         * this stage.
         */
        if ( EVP_EncryptFinal_ex(ctx, *ciphertext + len, &len) != 1 )
                error();
        
        // Clean up.
        EVP_CIPHER_CTX_free(ctx);
        
        return len;
}

/**
 * Generates an elliptic curve Diffie-Hellman secret out of a key pair.
 * The secret is SHA-256-hashed.
 * @return The secret length.
 */
int pingamate::crypto::ecdh(EVP_PKEY *private_key, EVP_PKEY *public_key, unsigned char **secret)
{
        EC_KEY *ec_private_key;
        EC_KEY *ec_public_key;
        unsigned char *tmp;
        int field_size;
        int secret_len;
        int secret_len_final;
        
        ec_private_key   = EVP_PKEY_get1_EC_KEY(private_key);
        ec_public_key    = EVP_PKEY_get1_EC_KEY(public_key);
        field_size       = 0;
        secret_len       = 0;
        secret_len_final = 0;
        tmp              = NULL;
        
        // Calculate the size of the buffer for the shared secret.
        field_size = EC_GROUP_get_degree(EC_KEY_get0_group(ec_private_key));
        secret_len = (field_size + 7) / 8;
        
        // Allocate the memory for the shared secret
        if ( !(tmp = static_cast<unsigned char *>(OPENSSL_malloc(secret_len))) )
                error();
        
        // Derive the shared secret.
        secret_len = ECDH_compute_key(tmp, secret_len, EC_KEY_get0_public_key(ec_public_key), ec_private_key, NULL);
        
        if ( secret_len <= 0 ) {
                OPENSSL_free(tmp);
                
                secret_len_final = -1;
        } else {
                /*
                 * Never use a derived secret directly. Typically, it is passed
                 * through some hash function to produce a key.
                 */
                *secret = static_cast<unsigned char *>(malloc(SHA256_DIGEST_LENGTH));
                
                pingamate::crypto::sha(tmp, secret_len, *secret);
                
                secret_len_final = SHA256_DIGEST_LENGTH;
        }
        
        return secret_len_final;
}
