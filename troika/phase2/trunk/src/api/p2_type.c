/*******************************************************************************

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

*******************************************************************************/

/*

[ba] destroy/delete, clone/copy


*/

#include "p2_type.h"

#include "util/p2_array.h"
#include "util/p2_hash_table.h"

#include <string.h>



// Default type interface members (use with caution)
void default__encode(void *p, char *buffer) { *buffer = '\0'; }
void *default__decode(char *buffer) { return NULL; }
void default__destroy(void *p) { }
void *default__clone(void *p) { return NULL; }



// Global variables for this module only.
p2_array *registered_types;
p2_hash_table *type_dictionary;
int total_types_ = 0;



// hidden for now.
void p2_type__delete(struct p2_type_itf *type)
{
    free(type->name);
    free(type);
}



p2_error p2_type_init()
{
    registered_types = p2_array__new(30, 2.0);
    type_dictionary = p2_hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    return P2_SUCCESS;
}



p2_error p2_type_end()
{
    p2_array__forall(registered_types, (void (*)(void *)) p2_type__delete);
    p2_array__delete(registered_types);
    p2_hash_table__delete(type_dictionary);

    return P2_SUCCESS;
}



p2_type p2_register_type(
    char *name,
    void (*encode)(void *, char *),
    void *(*decode)(char *),
    void (*destroy)(void *),
    void *(*clone)(void *))
{
    struct p2_type_itf *type = (struct p2_type_itf *) malloc(sizeof(struct p2_type_itf));
    type->name = strdup(name);

    type->encode = encode ? encode : default__encode;
    type->decode = decode ? decode : default__decode;
    type->destroy = destroy ? destroy : default__destroy;
    type->clone = clone ? clone : default__clone;

    // Important: the type with index 1 actually is at position 0 in the array.
    // The 0 index is avoided so that it can't be mistaken for a NULL.
    p2_array__enqueue(registered_types, (void *) type);
    p2_hash_table__add(type_dictionary, (void *) name, (void *) ++total_types_);

    return (p2_type) total_types_;
}



int p2_total_types()
{
    return total_types_;
}



p2_type p2_type_lookup(char *name)
{
    return (p2_type) p2_hash_table__lookup(type_dictionary, (void *) name);
}


//#include <stdio.h>
char *p2_type_name(p2_type type_index)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    return ((struct p2_type_itf *) (p2_array__get(registered_types, (int) type_index-1)))->name;
}


////////////////////////////////////////////////////////////////////////////////

/*
void p2_encode(void *p, p2_type *type, char *buffer)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    struct p2_type_itf *type = (struct p2_type_itf *) p2_array__get(registered_types, (int) type_index-1);
    type->encode(p, buffer);
}



void *p2_decode(p2_type type_index, char *buffer)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    p2_type *type = (p2_type *) p2_array__get(registered_types, (int) type - 1);
    return type->decode(buffer);
}



void p2_destroy(void *p, p2_type type)
{
//printf("+ p2_destroy: atom = %d, type = %d\n", (int) p, (int) type); fflush(stdout);
    p2_type *type = (p2_type *) p2_array__get(registered_types, (int) type - 1);
    type->destroy(p);
//printf("- p2_destroy\n"); fflush(stdout);
}



void *p2_clone(void *p, p2_type type)
{
    p2_type *type = (p2_type *) p2_array__get(registered_types, (int) type - 1);
    return type->clone(p);
}
*/


/*- end of file */
