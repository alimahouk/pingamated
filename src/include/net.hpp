//
//  net.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef NET_HPP
#define NET_HPP

#define PM_PORT         "1443"
#define TCP_BACKLOG     10
#define TCP_MAX_LENGTH  8192

#include <iostream>

namespace pingamate {
        namespace net {
                void accept_clients(void);
                void cleanup(void);
                void client_read_loop(const unsigned int socket);
                void setup_socket(void);
        }
}

#endif /* NET_HPP */
