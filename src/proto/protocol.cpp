//
//  protocol.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "protocol.hpp"

#include "pkc.hpp"

EVP_PKEY *server_key;

bool pingamate::proto::valid_transmission(const unsigned char *transmission)
{
        unsigned char magic_num[PM_MAGIC_NUM_LEN] = PM_MAGIC_NUM;
        
        if ( memcmp(transmission, magic_num, PM_MAGIC_NUM_LEN) == 0 )
                return true;
        
        return false;
}

uint32_t pingamate::proto::transmission_size(const unsigned char *transmission)
{
        uint32_t len;
        
        len = 0;
        
        if ( valid_transmission(transmission) ) {
                int index;
                
                index = PM_MAGIC_NUM_LEN;
                len   = (uint32_t)transmission[index + 3] |
                        ( (uint32_t)transmission[index + 2] << 8 ) |
                        ( (uint32_t)transmission[index + 1] << 16 ) |
                        ( (uint32_t)transmission[index] << 24 );// First 4 bytes contain the message length (excluding the magic number).
        }
        
        return len;
}

void pingamate::proto::parse_transmission(const int sender, const unsigned char *transmission)
{
        
}
