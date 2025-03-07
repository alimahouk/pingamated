//
//  util.cpp
//  pingamated
//
//  Created by Ali Mahouk on 12/24/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#include "util.hpp"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <uuid/uuid.h>
#include <zlib.h>

#include "types.hpp"
#include "sha.hpp"

unsigned char hex_val(const unsigned char c)
{
        /*
         * C++98 guarantees that '0', '1', ... '9' are consecutive.
         * It only guarantees that 'a' ... 'f' and 'A' ... 'F' are
         * in increasing order, but the only two alternative encodings
         * of the basic source character set that are still used by
         * anyone today (ASCII and EBCDIC) make them consecutive.
         */
        if ( '0' <= c && c <= '9' )
                return c - '0';
        else if ( 'a' <= c && c <= 'f' )
                return c - 'a' + 10;
        else if ( 'A' <= c && c <= 'F' )
                return c - 'A' + 10;
        else
                abort();
}

/**
 * Compresses a byte buffer.
 * @return The length of the compressed data, or -1 if an error
 * occured.
 */
unsigned long pingamate::util::compress(const unsigned char *src, unsigned long len, unsigned char **out)
{
        if ( out ) {
                *out = static_cast<unsigned char *>(malloc(ZLIB_CHUNK));
                
                if ( *out ) {
                        int ret;
                        z_stream stream;
                        
                        stream.zalloc    = Z_NULL;
                        stream.zfree     = Z_NULL;
                        stream.opaque    = Z_NULL;
                        stream.avail_in  = (uInt)len;
                        stream.avail_out = ZLIB_CHUNK;
                        stream.next_in   = (Bytef *)src;
                        stream.next_out  = (Bytef *)*out;
                        
                        deflateInit(&stream, Z_BEST_COMPRESSION);
                        
                        ret = deflate(&stream, Z_FINISH);
                        
                        deflateEnd(&stream);
                        
                        return stream.total_out;
                }
        }
        
        return -1;
}

/**
 * Decompresses a byte buffer.
 * @return The length of the decompressed data, or -1 if an error
 * occured.
 */
unsigned long pingamate::util::decompress(const unsigned char *src, unsigned long len, unsigned char **out)
{
        if ( out ) {
                *out = static_cast<unsigned char *>(malloc(ZLIB_CHUNK));
                
                if ( *out ) {
                        int ret;
                        z_stream stream;
                        
                        stream.zalloc    = Z_NULL;
                        stream.zfree     = Z_NULL;
                        stream.opaque    = Z_NULL;
                        stream.avail_in  = (uInt)len;
                        stream.avail_out = ZLIB_CHUNK;
                        stream.next_in   = (Bytef *)src;
                        stream.next_out  = (Bytef *)*out;
                        
                        inflateInit(&stream);
                        
                        ret = inflate(&stream, Z_NO_FLUSH);
                        
                        inflateEnd(&stream);
                        
                        return stream.total_out;
                }
        }
        
        return -1;
}

unsigned long pingamate::util::epoch_time(void)
{
        boost::posix_time::ptime const time_epoch(boost::gregorian::date(1970, 1, 1));
        
        return (boost::posix_time::second_clock::local_time() - time_epoch).total_seconds();
}

/**
 * Converts a byte array into a string.
 * @attention Do not use this on digests; use dtostr() instead.
 */
void pingamate::util::btostr(const unsigned char *uc, const size_t len, std::string &str)
{
        str.clear();
        str.reserve(len);
        
        char tmp[len * 2 + 1];
        
        for ( int i = 0; i < len; i++ )
                sprintf(&tmp[i * 2], "%02x", uc[i]);
        
        std::string hex(tmp);
        
        for ( std::string::const_iterator p = hex.begin(); p != hex.end(); p++ ) {
                unsigned char c = hex_val(*p);
                
                p++;
                
                if ( p == hex.end() )
                        break; // Incomplete last digit - should report error.
                
                c = (c << 4) + hex_val(*p); // + takes precedence over <<
                
                str.push_back(c);
        }
}

/**
 * Converts a string into a byte array.
 */
void pingamate::util::strtob(const std::string str, unsigned char **out)
{
        *out = static_cast<unsigned char *>(malloc(str.length()));
        
        memcpy(*out, str.data(), str.length());
}

/**
 * Generates a UUID & stores a hash of it in @p out.
 * @attention It is the caller's responsibility to
 * free the hash.
 */
void pingamate::util::uuid(unsigned char **out)
{
        if ( out ) {
                char *uuid_str;
                uuid_t id;
                
                *out     = static_cast<unsigned char *>(malloc(SHA256_DIGEST_LENGTH * sizeof(unsigned char)));
                uuid_str = static_cast<char *>(malloc(37)); // e.g. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + '\0'
                
                if ( uuid_str ) {
                        uuid_generate(id);
                        uuid_unparse_lower(id, uuid_str);
                        pingamate::crypto::sha_str(std::string(uuid_str), *out);
                        free(uuid_str);
                }
        }
}

void pingamate::util::uuid_str(std::string &str)
{
        unsigned char *hash;
        
        uuid(&hash);
        
        if ( hash ) {
                pingamate::crypto::dtostr(hash, SHA256_DIGEST_LENGTH, str);
                free(hash);
        }
}
