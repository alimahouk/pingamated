//
//  sha.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef SHA_HPP
#define SHA_HPP

#include <iostream>
#include <openssl/sha.h>

namespace pingamate {
        namespace crypto {
                int strtod(const std::string str, unsigned char **out);
                
                void dtostr(const unsigned char *bytes, int len, std::string &str);
                void sha(const unsigned char *data, size_t len, unsigned char digest[]);
                void sha_str(const std::string str, unsigned char digest[]);
        }
}

#endif /* SHA_HPP */
