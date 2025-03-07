//
//  pkc.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef PKC_HPP
#define PKC_HPP

#include <iostream>
#include <openssl/evp.h>

namespace pingamate {
        namespace crypto {
                EVP_PKEY* decode_key(unsigned char **c, size_t len);
                
                int dump_private_key(EVP_PKEY *pkey, const std::string path);
                int dump_public_key(EVP_PKEY *pkey, const std::string path);
                int encode_key(EVP_PKEY *key, unsigned char **dest);
                
                void fetch_private_key(const std::string path, EVP_PKEY **pkey);
                void fetch_public_key(const std::string path, EVP_PKEY **pkey);
                void gen_key(EVP_PKEY **pkey);
        }
}

#endif /* PKC_HPP */
