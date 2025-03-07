//
//  net.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "net.hpp"

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include "protocol.hpp"
#include "util.hpp"

std::unordered_map<int, std::string> ids;       // Key: the socket FD. Value: the identifier of the connected client.
std::unordered_map<std::string, int> sockets;   // Key: the identifier of the connected client. Value: the socket FD.

int acceptor_socket;

void pingamate::net::accept_clients(void)
{
        while ( 1 ) {
                struct sockaddr_storage client_address;
                std::string temp_id;
                std::thread thread_client;
                int new_client;
                socklen_t socket_size;
                
                pingamate::util::uuid_str(temp_id);
                
                socket_size = sizeof(client_address);
                new_client  = accept(acceptor_socket, (struct sockaddr *)&client_address, &socket_size);
                
                if ( new_client == -1 ) {
                        perror("accept");
                        continue;
                }
                
                ids[new_client]  = temp_id;
                sockets[temp_id] = new_client;
                thread_client    = std::thread(client_read_loop, new_client);
                
                thread_client.detach();
        }
}

void pingamate::net::cleanup(void)
{
        close(acceptor_socket);
}

void pingamate::net::client_read_loop(const unsigned int socket)
{
        std::vector<unsigned char> buffer(TCP_MAX_LENGTH);
        std::string id;
        ssize_t recv_len;
        uint32_t transmission_len;
        
        transmission_len = 0;
        
        while ( 1 ) {
                std::string text;
                
                recv_len += recv(socket, &buffer[0], buffer.size(), 0);
                
                if ( recv_len <= 0 ) {
                        if ( recv_len < 0 )
                                perror("recv");
                        
                        break;
                } else if ( recv_len > PM_MAGIC_NUM_LEN + 4) {
                        if ( transmission_len == 0 ) {
                                if ( pingamate::proto::valid_transmission(&buffer[0]) ) { // First, make sure we're dealing with our protocol.
                                        transmission_len = pingamate::proto::transmission_size(&buffer[0]);
                                        
                                        /*
                                         * But we have to account for the magic number's
                                         * length when resizing the buffer because it's
                                         * already in the buffer at this point.
                                         */
                                        buffer.resize(transmission_len + PM_MAGIC_NUM_LEN);
                                } else {
                                        close(socket); // GTFO.
                                        
                                        break;
                                }
                        } else if ( recv_len == transmission_len ) { // Received the full message.
                                // Reset these.
                                recv_len         = 0;
                                transmission_len = 0;
                                
                                buffer.clear(); // Clear out the TCP buffer.
                                pingamate::proto::parse_transmission(socket, &buffer[0]);
                        }
                }
        }
        
        /*
         * If the loop terminates, that means the socket is
         * no longer valid, so we remove it.
         */
        id = ids[socket];
        
        ids.erase(socket);
        sockets.erase(id);
}

void pingamate::net::setup_socket(void)
{
        struct addrinfo *p;
        struct addrinfo *server_info;
        struct addrinfo  hints;
        int flag_true;
        int ret_val;
        
        flag_true = 1;
        
        memset(&hints, 0, sizeof(hints));
        
        hints.ai_family   = PF_INET;
        hints.ai_flags    = AI_PASSIVE;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_socktype = SOCK_STREAM;
        
        if ( (ret_val = getaddrinfo(NULL, PM_PORT, &hints, &server_info)) != 0 ) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret_val));
                
                return;
        }
        
        for ( p = server_info; p != NULL; p = p->ai_next ) {
                if ( (acceptor_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
                        perror("server: socket");
                        
                        continue;
                }
                
                if ( setsockopt(acceptor_socket, IPPROTO_TCP, TCP_NODELAY, &flag_true, sizeof(int)) == -1 ) {
                        perror("setsockopt");
                        exit(1);
                }
                
                if ( setsockopt(acceptor_socket, SOL_SOCKET, SO_REUSEADDR, &flag_true, sizeof(int)) == -1 ) {
                        perror("setsockopt");
                        exit(1);
                }
                
                if ( bind(acceptor_socket, p->ai_addr, p->ai_addrlen) == -1 ) {
                        close(acceptor_socket);
                        perror("server: bind");
                        
                        continue;
                }
                
                break;
        }
        
        if ( !p )  {
                fprintf(stderr, "server: failed to bind\n");
                exit(1);
        }
        
        if ( listen(acceptor_socket, TCP_BACKLOG) == -1 ) {
                perror("listen");
                exit(1);
        }
        
        freeaddrinfo(server_info);
}
