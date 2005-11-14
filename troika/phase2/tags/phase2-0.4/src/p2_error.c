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

#include <string.h>
#include <stdio.h>

#include "p2_error.h"
#include "util/array.h"
#include "util/hash_table.h"
//#include "util/sequence.h"



// Global vars for this module only.
ARRAY *registered_errors_;
HASH_TABLE *error_dictionary_;
int total_errors_ = 0;


/*
struct error_wrapper
{
    char *name;
    SEQUENCE *seq;
};*/



P2_error P2_error_init()
{
    P2_error_type = P2_register_type(P2_ERROR_NAME, NULL, NULL, NULL, NULL);    

    registered_errors_ = array__new(30, 2.0);
    error_dictionary_ = hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    P2_SUCCESS = NULL;

    // Bad token syntax
    INVALID_TOKEN = P2_register_error("INVALID_TOKEN");
    INVALID_IDENTIFIER = P2_register_error("INVALID_IDENTIFIER");
    INVALID_COMMAND_NAME = P2_register_error("INVALID_COMMAND_NAME");

    // Item not found
    UNKNOWN_COMMAND = P2_register_error("UNKNOWN_COMMAND");
    UNKNOWN_SYMBOL = P2_register_error("UNKNOWN_SYMBOL");
    UNKNOWN_VARIABLE = P2_register_error("UNKNOWN_VARIABLE");
    UNKNOWN_DATASET = P2_register_error("UNKNOWN_DATASET");

    // Bad command call
    BAD_ARGUMENT = P2_register_error("BAD_ARGUMENT");
    TOO_FEW_ARGUMENTS = P2_register_error("TOO_FEW_ARGUMENTS");
    TOO_MANY_ARGUMENTS = P2_register_error("TOO_MANY_ARGUMENTS");

    REDUX_FAILURE = P2_register_error("REDUX_FAILURE");
    
    // Program-level errors
    DUPLICATE_COMMAND_NAME = P2_register_error("DUPLICATE_COMMAND_NAME");
    BAD_REGEX = P2_register_error("BAD_REGEX");
    UNEXPECTED_VALUE = P2_register_error("UNEXPECTED_VALUE");
    
    return P2_SUCCESS;
}



P2_error P2_error_end()
{
    array__forall(registered_errors_, (void (*)(void *)) free);
    array__delete(registered_errors_);
    hash_table__delete(error_dictionary_);

    return P2_SUCCESS;
}



// Note: there is nothing to prevent duplicate error names.  This is appropriate
// because the name is just a descriptor, and may even be omitted if it will not
// be used.  The actual P2_error code is what uniquely identifies the error.
P2_error P2_register_error(char *name)
{
    char *s = strdup(name);
    
    // Important: the error with index 1 actually is at position 0 in the array.
    // The 0 index is avoided so that it can't be mistaken for a NULL.
    array__enqueue(registered_errors_, (void *) s);
    hash_table__add(error_dictionary_, (void *) s, (void *) ++total_errors_);

    /*
    if (!all_errors_)
        all_errors_ = sequence__new((void *) P2_error_type, (void *) total_errors_);
    else
        all_errors_ = sequence__cat(all_errors_, sequence__new((void *) P2_error_type, (void *) total_errors_);
    */
    
    return (P2_type) total_errors_;
}



void P2_print_error(P2_error err, char *buffer)
{
    if (err)
        //printf("\t>> Error: %s <<\n", P2_errors[err]);
        sprintf(buffer, (char *) array__get(registered_errors_, (int) err-1));
}



/*- end of file */
