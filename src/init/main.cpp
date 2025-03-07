//
//  main.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "crypto.hpp"
#include "fs.hpp"
#include "net.hpp"
#include "pkc.hpp"
#include "sha.hpp"

void cleanup(void)
{
        pingamate::crypto::cleanup();
        pingamate::net::cleanup();
}

void handle_signals(int signal)
{
        switch ( signal ) {
                case SIGINT:
                case SIGQUIT:
                case SIGTERM:
                        exit(0);
                        
                        break;
                        
                default:
                        break;
        }
}

void bootstrap(void)
{
        EVP_PKEY *server_key;
        char c1;
        
        c1 = 'a';
        
        // Verify directory structure.
        if ( pingamate::fs::dir_exists(pingamate::fs::dpath_root()) == 0 )
                pingamate::fs::make_dir(pingamate::fs::dpath_root());
        
        if ( pingamate::fs::dir_exists(pingamate::fs::dpath_users()) == 0 )
                pingamate::fs::make_dir(pingamate::fs::dpath_users());
        
        // Verify user groups.
        for ( int i = 0; i < 36; i++ ) {
                
                char c2;
                
                c2 = 'a';
                
                if ( i == 26 )
                        c1 = '0';
                
                for ( int j = 0; j < 36; j++ ) {
                        std::string user_group;
                        
                        if ( j == 26 )
                                c2 = '0';
                        
                        user_group.push_back(c1);
                        user_group.push_back(c2);
                        
                        if ( pingamate::fs::dir_exists(pingamate::fs::dpath_users() + "/" + user_group) == 0 )
                                pingamate::fs::make_dir(pingamate::fs::dpath_users() + "/" + user_group);
                        
                        c2++;
                }
                
                c1++;
        }
        
        // Check for the server's keys.
        pingamate::crypto::fetch_private_key(pingamate::fs::fpath_server_private_key(), &server_key);
        
        if ( !server_key ) {
                printf("Generating fresh server keys!\n");
                
                pingamate::crypto::gen_key(&server_key);
                pingamate::crypto::dump_private_key(server_key, pingamate::fs::fpath_server_private_key());
                pingamate::crypto::dump_public_key(server_key, pingamate::fs::fpath_server_public_key());
        }
}

void print_key_checksum(void)
{
        EVP_PKEY *server_key;
        unsigned char *encoding;
        unsigned char checksum[SHA256_DIGEST_LENGTH];
        std::string hash;
        int key_len;
        
        pingamate::crypto::fetch_public_key(pingamate::fs::fpath_server_public_key(), &server_key);
        
        key_len = pingamate::crypto::encode_key(server_key, &encoding);
        
        pingamate::crypto::sha(encoding, key_len, checksum);
        pingamate::crypto::dtostr(checksum, SHA256_DIGEST_LENGTH, hash);
        std::cout << "Public key checksum: " << hash << std::endl;
}

int main(int argc, const char *argv[])
{
        signal(SIGINT, handle_signals);
        signal(SIGQUIT, handle_signals);
        signal(SIGTERM, handle_signals);  // Catch termination signals to clean up first.
        atexit(cleanup);
        pingamate::crypto::init();
        bootstrap();
        //print_key_checksum();
        pingamate::net::setup_socket();
        pingamate::net::accept_clients();
        
        return 0;
}
