/**
    \file p2_error.h

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_ERROR_H
#define P2_ERROR_H


#include "p2_flags.h"


/** A unique 32-bit value to identify a specific error type. */
typedef void * p2_error;


// Error codes for a variety of contexts (custom errors may be created with
// p2_register_error).
p2_error

    P2_SUCCESS,

    // Bad token syntax
    INVALID_TOKEN,
    INVALID_IDENTIFIER,
    INVALID_COMMAND_NAME,  // properly, a program-level error.

    // Item not found
    UNKNOWN_COMMAND,
    UNKNOWN_SYMBOL,
    UNKNOWN_VARIABLE,
    UNKNOWN_DATASET,

    // Bad command call
    BAD_ARGUMENT,
    TOO_FEW_ARGUMENTS,
    TOO_MANY_ARGUMENTS,

    REDUX_FAILURE,

    // Program-level errors
    DUPLICATE_COMMAND_NAME,
    BAD_REGEX,
    UNEXPECTED_VALUE;


/**  */
p2_error p2_error_init();

/**  */
p2_error p2_error_end();

/**
    \note  There is nothing to prevent duplicate error names.  The name is just
    a helpful description, and may even be omitted if it will not be used.  The
    actual p2_error code is what uniquely identifies the error. */
p2_error p2_error__new(char *name);

/**  */
void p2_error__encode(p2_error err, char *buffer);


#endif  // P2_ERROR_H

