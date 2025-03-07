//
//  pkc.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "pkc.hpp"

#include <cassert>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "crypto.hpp"
#include "fs.hpp"

/**
 * Converts DER data into a public key.
 * @return The key.
 */
EVP_PKEY* pingamate::crypto::decode_key(unsigned char **c, size_t len)
{
        EVP_PKEY *key;
        const unsigned char *q;
        
        q = *c;
        
        key = d2i_PUBKEY(NULL, &q, len);
        
        return key;
}

/**
 * Saves the given key to the disk.
 * @return 0 on success, 1 if the file could not be created, or -1 if there
 * was a problem with the passed key.
 */
int pingamate::crypto::dump_private_key(EVP_PKEY *pkey, const std::string path)
{
        const BIGNUM *private_key;
        EC_KEY *ec_key;
        FILE *f_key;
        int err_no;
        
        ec_key = EVP_PKEY_get1_EC_KEY(pkey);
        err_no = 0;
        
        if ( ec_key ) {
                f_key       = pingamate::fs::make_file(path);
                private_key = EC_KEY_get0_private_key(ec_key);
                
                if ( f_key ) {
                        if ( private_key ) {
                                if ( !PEM_write_ECPrivateKey(f_key, ec_key, NULL, NULL, 0, NULL, NULL) ) {
                                        error();
                                        
                                        err_no = EXIT_FAILURE;
                                }
                        } else {
                               err_no = -1;
                        }
                        
                        EC_KEY_free(ec_key);
                        fclose(f_key);
                } else {
                        err_no = 1;
                }
        } else {
                err_no = -1;
        }
        
        return err_no;
}

/**
 * Saves the given key to the disk.
 * @return 0 on success, 1 if the file could not be created, or -1 if there
 * was a problem with the passed key.
 */
int pingamate::crypto::dump_public_key(EVP_PKEY *pkey, const std::string path)
{
        EC_KEY *ec_key;
        FILE *f_key;
        int err_no;
        
        ec_key = EVP_PKEY_get1_EC_KEY(pkey);
        err_no = 0;
        
        if ( ec_key ) {
                f_key = pingamate::fs::make_file(path);
                
                if ( f_key ) {
                        if ( !PEM_write_EC_PUBKEY(f_key, ec_key) ) {
                                error();
                                
                                err_no = EXIT_FAILURE;
                        }
                        
                        EC_KEY_free(ec_key);
                        fclose(f_key);
                } else {
                        err_no = 1;
                }
        } else {
                err_no = -1;
        }
        
        return err_no;
}

/**
 * Converts a public key into DER format.
 * @return The length of the data, or -1 if an error occurred.
 */
int pingamate::crypto::encode_key(EVP_PKEY *key, unsigned char **dest)
{
        int key_len;
        
        if ( dest ) {
                unsigned char *temp_buffer;
                
                key_len     = i2d_PUBKEY(key, NULL);
                *dest       = static_cast<unsigned char *>(malloc(key_len));
                temp_buffer = *dest;
                
                i2d_PUBKEY(key, &temp_buffer);
        }
        
        return key_len;
}

void pingamate::crypto::fetch_private_key(const std::string path, EVP_PKEY **pkey)
{
        if ( pkey ) {
                EC_KEY *ec_key;
                FILE *f_key;
                
                ec_key = NULL;
                f_key  = pingamate::fs::open_file(path);
                *pkey  = NULL;
                
                if ( f_key ) {
                        ec_key = PEM_read_ECPrivateKey(f_key, NULL, NULL, NULL);
                        
                        if ( ec_key )
                                assert(EC_KEY_check_key(ec_key) == 1);
                        
                        fclose(f_key);
                }
                
                if ( ec_key ) {
                        *pkey = EVP_PKEY_new();
                        
                        assert(EVP_PKEY_assign_EC_KEY(*pkey, ec_key) == 1);
                }
        }
}

void pingamate::crypto::fetch_public_key(const std::string path, EVP_PKEY **pkey)
{
        if ( pkey ) {
                EC_KEY *ec_key;
                FILE *f_key;
                
                ec_key = NULL;
                f_key  = pingamate::fs::open_file(path);
                *pkey  = NULL;
                
                if ( f_key ) {
                        ec_key = PEM_read_EC_PUBKEY(f_key, NULL, NULL, NULL);
                        
                        /*
                         * If this is a private key, don't bother trying to open
                         * the public key's file (doesn't exist anyway). The public
                         * key is computed using the private key.
                         */
                        if ( ec_key )
                                assert(EC_KEY_check_key(ec_key) == 1);
                        
                        fclose(f_key);
                }
                
                if ( ec_key ) {
                        *pkey = EVP_PKEY_new();
                        
                        assert(EVP_PKEY_assign_EC_KEY(*pkey, ec_key) == 1);
                }
        }
}

void pingamate::crypto::gen_key(EVP_PKEY **pkey)
{
        BIO *bio;
        EC_KEY *ec_key;
        
        bio    = BIO_new(BIO_s_mem());
        ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        *pkey  = EVP_PKEY_new();
        
        EC_KEY_set_asn1_flag(ec_key, OPENSSL_EC_NAMED_CURVE);
        
        // Create the public/private EC key pair here.
        if ( !(EC_KEY_generate_key(ec_key)) )
                BIO_printf(bio, "crypto_gen_keys: error generating the ECC key.");
        
        /*
         * Converting the EC key into a PKEY structure lets us
         * handle the key just like any other key pair.
         */
        if ( !EVP_PKEY_assign_EC_KEY(*pkey, ec_key) )
                BIO_printf(bio, "crypto_gen_keys: error assigning ECC key to EVP_PKEY structure.");
        
        // Cleanup.
        BIO_free_all(bio);
}
