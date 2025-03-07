//
//  protocol.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#define PM_MAGIC_NUM            {0x89, 0x50, 0x44, 0x48, 0x5A, 0x0d, 0x0a, 0x1a, 0x0a}
#define PM_MAGIC_NUM_LEN        9
#define PM_PROTO_VERSION        1

#include <iostream>

namespace pingamate {
        namespace proto {
                bool valid_transmission(const unsigned char *transmission);
                
                uint32_t transmission_size(const unsigned char *transmission);
                
                void parse_transmission(const int sender, const unsigned char *transmission);
        }
}

#endif /* PROTOCOL_HPP */
