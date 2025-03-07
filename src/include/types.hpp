//
//  types.hpp
//  pingamated
//
//  Created by Ali Mahouk on 12/23/16.
//  Copyright © 2016 Ali Mahouk. All rights reserved.
//

#ifndef TYPES_HPP
#define TYPES_HPP

/*
 * Slots 1 -> 16 are reserved for server messages.
 */
typedef enum {
        tm_none = 0,
        tm_error = 1,
        tm_greeting,
        tm_latest_state
} transmission_t;

/*
 * STRUCTURE PACKING
 * The struct members are ordered in such a way
 * that slop is minimized. This is crucial as it
 * helps reduce the memory footprint when scaling
 * up.
 */

#endif /* TYPES_HPP */
