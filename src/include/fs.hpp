//
//  fs.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef FS_HPP
#define FS_HPP

#define DIR_ROOT                "pingamated"
#define DIR_USERS               "u"
#define DIR_USER_FILES          "f"
#define FILE_SERVER_PRIVATE_KEY "server_private"
#define FILE_SERVER_PUBLIC_KEY  "server_public"
#define FILE_USER_INBOX         "inbox"
#define FILE_USER_KEY           "pub"
#define PM_FILE_EXTENSION       "ping"

#include <iostream>

namespace pingamate {
        namespace fs {
                FILE* make_file(const std::string path);
                FILE* open_file(const std::string path);
                
                std::string dpath_root(void);
                std::string dpath_user(const std::string user_id);
                std::string dpath_user_files(const std::string user_id);
                std::string dpath_users(void);
                std::string fpath_server_private_key(void);
                std::string fpath_server_public_key(void);
                std::string fpath_user_inbox(const std::string user_id);
                std::string fpath_user_key(const std::string user_id);
                
                int dir_exists(const std::string path);
                int file_exists(const std::string path);
                int make_dir(const std::string path);
                int remove_file(const std::string path);
                
                off_t freadb(const std::string path, unsigned char **buffer);
                off_t fwriteb(const std::string path, const unsigned char *buffer, const size_t size);
                
                void freadt(const std::string path, std::string &buffer);
                void fwritet(const std::string path, const std::string buffer);
        }
}

#endif /* FS_HPP */
