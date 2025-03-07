//
//  util.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef UTIL_HPP
#define UTIL_HPP

#define ZLIB_CHUNK      16384

#include <iostream>

namespace pingamate {
        namespace util {
                unsigned long compress(const unsigned char *src, unsigned long len, unsigned char **out);
                unsigned long decompress(const unsigned char *src, unsigned long len, unsigned char **out);
                unsigned long epoch_time(void);
                
                void btostr(const unsigned char *uc, const size_t len, std::string &str);
                void strtob(const std::string str, unsigned char **out);
                void uuid(unsigned char **out);
                void uuid_str(std::string &str);
        }
}

#endif /* UTIL_HPP */
