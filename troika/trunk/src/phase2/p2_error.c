/*+

  p2_error.c

  last edited: 5/31/05
  
*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*///////////////////////////////////////////////////////////////////////////////

#include <string.h>  // strdup
#include <stdio.h>   // free, sprintf

#include "p2_error.h"

#include "util/array.h"
#include "util/hash_table.h"




// Global vars for this module only.
P2_array *registered_errors_;
P2_hash_table *error_dictionary_;
int total_errors_ = 0;



P2_error P2_error_init()
{

    registered_errors_ = array__new(30, 2.0);
    error_dictionary_ = hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    P2_SUCCESS = NULL;

    // Bad token syntax
    INVALID_TOKEN = P2_error__new("INVALID_TOKEN");
    INVALID_IDENTIFIER = P2_error__new("INVALID_IDENTIFIER");
    INVALID_COMMAND_NAME = P2_error__new("INVALID_COMMAND_NAME");

    // Item not found
    UNKNOWN_COMMAND = P2_error__new("UNKNOWN_COMMAND");
    UNKNOWN_SYMBOL = P2_error__new("UNKNOWN_SYMBOL");
    UNKNOWN_VARIABLE = P2_error__new("UNKNOWN_VARIABLE");
    UNKNOWN_DATASET = P2_error__new("UNKNOWN_DATASET");

    // Bad command call
    BAD_ARGUMENT = P2_error__new("BAD_ARGUMENT");
    TOO_FEW_ARGUMENTS = P2_error__new("TOO_FEW_ARGUMENTS");
    TOO_MANY_ARGUMENTS = P2_error__new("TOO_MANY_ARGUMENTS");

    REDUX_FAILURE = P2_error__new("REDUX_FAILURE");

    // Program-level errors
    DUPLICATE_COMMAND_NAME = P2_error__new("DUPLICATE_COMMAND_NAME");
    BAD_REGEX = P2_error__new("BAD_REGEX");
    UNEXPECTED_VALUE = P2_error__new("UNEXPECTED_VALUE");

    return P2_SUCCESS;
}



P2_error P2_error_end()
{
    array__forall(registered_errors_, (void (*)(void *)) free);
    array__delete(registered_errors_);
    hash_table__delete(error_dictionary_);

    return P2_SUCCESS;
}



P2_error P2_error__new(char *name)
{
    char *s = strdup(name);

    // Important: the error with index 1 actually is at position 0 in the array.
    // The 0 index is avoided so that it can't be mistaken for a NULL.
    array__enqueue(registered_errors_, (void *) s);
    hash_table__add(error_dictionary_, (void *) s, (void *) ++total_errors_);

    P2_error err = (P2_error) total_errors_;

    return err;
}



void P2_error__encode(P2_error err, char *buffer)
{
    if (err)
        //printf("\t>> Error: %s <<\n", P2_errors[err]);
        sprintf(buffer, (char *) array__get(registered_errors_, (int) err - 1));
}


