//
//  sha.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "sha.hpp"

#include <iomanip>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sstream>

#include "crypto.hpp"
#include "util.hpp"

/**
 * Converts a digest string into a byte array.
 * Output buffer should be cleared by the caller.
 */
int pingamate::crypto::strtod(const std::string str, unsigned char **out)
{
        BIGNUM *input;
        int input_length;
        
        input        = BN_new();
        input_length = BN_hex2bn(&input, str.c_str());
        input_length = (input_length + 1) / 2; // BN_hex2bn() returns the number of hex digits.
        *out         = static_cast<unsigned char *>(malloc(input_length));
        
        BN_bn2bin(input, *out);
        BN_free(input);
        
        return input_length;
}

/**
 * Converts a byte array containing a digest into its string representation.
 */
void pingamate::crypto::dtostr(const unsigned char *bytes, int len, std::string &str)
{
        BIGNUM *output;
        
        output = BN_new();
        
        BN_bin2bn(bytes, len, output);
        
        str = std::string(BN_bn2hex(output));
        
        BN_free(output);
}

/**
 * Generates the double SHA-256 hash of the given binary data.
 * @attention @param digest should be of size SHA256_DIGEST_LENGTH.
 */
void pingamate::crypto::sha(const unsigned char *data, size_t len, unsigned char digest[])
{
        unsigned char tmp[SHA256_DIGEST_LENGTH];
        
        SHA256(data, len, tmp);
        SHA256(tmp, SHA256_DIGEST_LENGTH, digest);
}

/**
 * Generates the SHA-256 hash of the given string.
 * @attention @param digest should be of size SHA256_DIGEST_LENGTH.
 */
void pingamate::crypto::sha_str(const std::string str, unsigned char digest[])
{
        unsigned char *tmp;
        
        pingamate::util::strtob(str, &tmp);
        sha(tmp, str.length(), digest);
}
