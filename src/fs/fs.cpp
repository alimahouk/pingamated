//
//  fs.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "fs.hpp"

#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

FILE* pingamate::fs::make_file(const std::string path)
{
        if ( !path.empty() )
                return fopen(path.c_str(), "w+");
        else
                return NULL;
}

/**
 * Returns the handle of the given file (if it exists) for reading.
 * @return The file handle.
 */
FILE* pingamate::fs::open_file(const std::string path)
{
        if ( !path.empty() )
                return fopen(path.c_str(), "r");
        else
                return NULL;
}

std::string pingamate::fs::dpath_root(void)
{
        return DIR_ROOT;
}

std::string pingamate::fs::dpath_user(const std::string user_id)
{
        if ( !user_id.empty() )
                return dpath_root() + "/" + DIR_USERS + "/" + user_id.substr(0, 2) + "/" + user_id;
        else
                return "";
}

std::string pingamate::fs::dpath_user_files(const std::string user_id)
{
        if ( !user_id.empty() )
                return dpath_user(user_id) + "/" + DIR_USER_FILES;
        else
                return "";
}

std::string pingamate::fs::dpath_users(void)
{
        return dpath_root() + "/" + DIR_USERS;
}

std::string pingamate::fs::fpath_server_private_key(void)
{
        return dpath_root() + "/" + FILE_SERVER_PRIVATE_KEY + ".pem";
}

std::string pingamate::fs::fpath_server_public_key(void)
{
        return dpath_root() + "/" + FILE_SERVER_PUBLIC_KEY + ".pem";
}

std::string pingamate::fs::fpath_user_inbox(const std::string user_id)
{
        if ( !user_id.empty() )
                return dpath_user(user_id) + "/" + FILE_USER_INBOX + "." + PM_FILE_EXTENSION;
        else
                return "";
}

std::string pingamate::fs::fpath_user_key(const std::string user_id)
{
        if ( !user_id.empty() )
                return dpath_user(user_id) + "/" + FILE_USER_KEY + ".pem";
        else
                return "";
}

int pingamate::fs::dir_exists(const std::string path)
{
        DIR *dir;
        
        dir = opendir(path.c_str());
        
        if ( dir ) {
                closedir(dir);
                
                return 1;
        } else if ( ENOENT == errno ) {
                return 0;
        } else {
                perror("dir_exists");
                
                return -1;
        }
}

int pingamate::fs::file_exists(const std::string path)
{
        if( access(path.c_str(), F_OK) != -1 )
                return 1;
        else
                return 0;
}

int pingamate::fs::make_dir(const std::string path)
{
        return mkdir(path.c_str(), 0700);
}

/**
 * Deletes the file at the given path.
 * @return 0 if the file was removed successfuly, -1 otherwise.
 */
int pingamate::fs::remove_file(const std::string path)
{
        return remove(path.c_str());
}

/**
 * Reads the binary file at the given path into the given buffer. It is the
 * caller's duty to free the buffer.
 * @return The number of bytes read.
 */
off_t pingamate::fs::freadb(const std::string path, unsigned char **buffer)
{
        off_t size = 0;
        
        if ( buffer ) {
                *buffer = NULL;
                
                std::ifstream file(path, std::ios::binary | std::ios::ate);
                
                if ( file.is_open() ) {
                        size = file.tellg();
                        *buffer = static_cast<unsigned char *>(malloc(size));
                        
                        file.seekg(0, std::ios::beg);
                        file.read(reinterpret_cast<char *>(*buffer), size);
                }
        }
        
        return size;
}

off_t pingamate::fs::fwriteb(const std::string path, const unsigned char *buffer, const size_t size)
{
        off_t bytes_written = 0;
        
        if ( buffer ) {
                FILE *file = fopen(path.c_str(), "wb");
                
                
                if ( file ) {
                        bytes_written = fwrite(buffer, 1, size, file);
                        fclose(file);
                }
        }
        
        return bytes_written;
}

void pingamate::fs::freadt(const std::string path, std::string &buffer)
{
        char c;
        std::ifstream file(path);
        
        if ( file.is_open() )
                while ( file.get(c) )
                        buffer += c;
}

void pingamate::fs::fwritet(const std::string path, const std::string buffer)
{
        std::ofstream file(path);
        
        if ( file.is_open() )
                file << buffer;
}
