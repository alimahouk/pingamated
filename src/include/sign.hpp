//
//  sign.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef SIGN_HPP
#define SIGN_HPP

#include <openssl/ssl.h>

namespace pingamate {
        namespace crypto {
                ECDSA_SIG* decode_sign(unsigned char **c, size_t len);
                ECDSA_SIG* ecdsa_sign(const unsigned char *hash, int hash_len, EVP_PKEY *pkey);
                
                int ecdsa_verify(const unsigned char *hash, int hash_len, ECDSA_SIG *sign, EVP_PKEY *pkey);
                int encode_sign(const ECDSA_SIG *sign, unsigned char **buf);
        }
}

#endif /* SIGN_HPP */
