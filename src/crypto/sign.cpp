//
//  sign.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "sign.hpp"

#include <openssl/conf.h>
#include <openssl/ecdsa.h>

/**
 * Decodes a DER-encoded ECDSA signature.
 * @return A pointer to the ECDSA_SIG structure or NULL.
 */
ECDSA_SIG* pingamate::crypto::decode_sign(unsigned char **c, size_t len)
{
        const unsigned char *q;
        
        q = *c;
        
        return d2i_ECDSA_SIG(NULL, &q, len);
}

/**
 * Signs the given hash with the given private key.
 */
ECDSA_SIG* pingamate::crypto::ecdsa_sign(const unsigned char *hash, int hash_len, EVP_PKEY *pkey)
{
        EC_KEY *ec_key = EVP_PKEY_get1_EC_KEY(pkey);
        
        return ECDSA_do_sign(hash, hash_len, ec_key);
}

/**
 * Verfies that a signature was created by the private half of the given
 * public key.
 * @return 1 if the signature is okay, 0 if the signature is incorrect, or
 * -1 if an error occured.
 */
int pingamate::crypto::ecdsa_verify(const unsigned char *hash, int hash_len, ECDSA_SIG *sign, EVP_PKEY *pkey)
{
        EC_KEY *ec_key;
        
        ec_key = EVP_PKEY_get1_EC_KEY(pkey);
        
        return ECDSA_do_verify(hash, hash_len, sign, ec_key);
}

/**
 * DER-encodes the contents of ECDSA_SIG object.
 * @return The length of the DER encoded ECDSA_SIG object or 0.
 */
int pingamate::crypto::encode_sign(const ECDSA_SIG *sign, unsigned char **buf)
{
        return i2d_ECDSA_SIG(sign, buf);
}
