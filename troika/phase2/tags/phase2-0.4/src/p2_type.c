/*+

  p2_type.c

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

/*

[ba] destroy/delete, clone/copy

["] factory method

[N] Consider sorting DOM data sets.
*/



#include <string.h>

#include "p2_type.h"

#include "util/array.h"
#include "util/hash_table.h"



struct P2_type_itf
{
    char *name;
    void (*encode)(P2_atom, char *);
    P2_atom (*decode)(char *);
    void (*destroy)(P2_atom);
    P2_atom (*clone)(P2_atom);
};



// Default type interface members (use with caution)
void default__encode(P2_atom p, char *buffer) { *buffer = '\0'; }
P2_atom default__decode(char *buffer) { return (P2_atom) NULL; }
void default__destroy(P2_atom p) { }
P2_atom default__clone(P2_atom p) { return (P2_atom) NULL; }



// Global vars for this module only.
ARRAY *registered_types_;
HASH_TABLE *type_dictionary_;
int total_types_ = 0;



// hidden for now.
void P2_type__delete(struct P2_type_itf *type)
{
    free(type->name);
    free(type);
}



P2_error P2_type_init()
{
    registered_types_ = array__new(30, 2.0);
    type_dictionary_ = hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    return P2_SUCCESS;
}



P2_error P2_type_end()
{
    array__forall(registered_types_, (void (*)(void *)) P2_type__delete);
    array__delete(registered_types_);
    hash_table__delete(type_dictionary_);

    return P2_SUCCESS;
}



P2_type P2_register_type(
    char *name,
    void (*encode)(P2_atom, char *),
    P2_atom (*decode)(char *),
    void (*destroy)(P2_atom),
    P2_atom (*clone)(P2_atom))
{
    struct P2_type_itf *type = (struct P2_type_itf *) malloc(sizeof(struct P2_type_itf));
    type->name = strdup(name);

    type->encode = encode ? encode : default__encode;
    type->decode = decode ? decode : default__decode;
    type->destroy = destroy ? destroy : default__destroy;
    type->clone = clone ? clone : default__clone;

    // Important: the type with index 1 actually is at position 0 in the array.
    // The 0 index is avoided so that it can't be mistaken for a NULL.
    array__enqueue(registered_types_, (void *) type);
    hash_table__add(type_dictionary_, (void *) name, (void *) ++total_types_);

    return (P2_type) total_types_;
}



int P2_total_types()
{
    return total_types_;
}



P2_type P2_type_lookup(char *name)
{
    return (P2_type) hash_table__lookup(type_dictionary_, (void *) name);
}


//#include <stdio.h>
char *P2_type_name(P2_type type_index)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    return ((struct P2_type_itf *) (array__get(registered_types_, (int) type_index-1)))->name;
}



void P2_encode(P2_atom p, P2_type type_index, char *buffer)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    struct P2_type_itf *type = (struct P2_type_itf *) array__get(registered_types_, (int) type_index-1);
    type->encode(p, buffer);
}



P2_atom P2_decode(P2_type type_index, char *buffer)
{
    // Warning: no checking of array bounds... this had better be a registered type.
    struct P2_type_itf *type = (struct P2_type_itf *) array__get(registered_types_, (int) type_index-1);
    return type->decode(buffer);
}



void P2_destroy(P2_atom p, P2_type type)
{
//printf("+ P2_destroy: atom = %d, type = %d\n", (int) p, (int) type); fflush(stdout);
    struct P2_type_itf *itf = (struct P2_type_itf *) array__get(registered_types_, (int) type-1);
    itf->destroy(p);
//printf("- P2_destroy\n"); fflush(stdout);
}



P2_atom P2_clone(P2_atom p, P2_type type)
{
    struct P2_type_itf *itf = (struct P2_type_itf *) array__get(registered_types_, (int) type-1);
    return itf->clone(p);
}



/*- end of file */
