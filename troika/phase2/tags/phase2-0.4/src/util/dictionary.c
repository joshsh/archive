/*+

  dictionary.c

  last edited: 6/25/05

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

//#include "util/array.h"
#include "util/hash_table.h"

//#include <stdio.h>



HASH_TABLE *dictionary_;



// P2_primitive type interface /////////////////////////////////////////////////



void P2_primitive__encode(struct P2_primitive *p, char *buffer)
{
    sprintf(buffer, p->name);
}



struct P2_primitive *P2_primitive__decode(char *buffer)
{
    return P2_lookup_primitive(buffer);
}



void P2_primitive__delete(struct P2_primitive *prim)
{
    free(prim->name);
    free(prim->parameter_types);
    if (prim->transparency)
        free(prim->transparency);
    if (prim->parameter_names)
        free(prim->parameter_names);
    free(prim);
}



////////////////////////////////////////////////////////////////////////////////



P2_error P2_primitive_init()
{
    P2_primitive_type = P2_register_type(P2_PRIMITIVE_NAME,
        (ENCODE_FORMAT) P2_primitive__encode,
        (DECODE_FORMAT) P2_primitive__decode,
        //(DESTROY_FORMAT) P2_primitive__delete,
	(DESTROY_FORMAT) NULL,
        (CLONE_FORMAT) NULL);

    registered_primitives_ = array__new(30, 2.0);
    primitives_dictionary_ = hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    return P2_SUCCESS;
}



// Caution: primitives are garbage-collected in this library although they
// will have been constructed outside of it.
P2_error P2_primitive_end()
{
        array__forall(registered_primitives_, (void (*)(void *)) P2_primitive__delete);
        array__delete(registered_primitives_);
        hash_table__delete(primitives_dictionary_);

        return P2_SUCCESS;
}



void P2_register_primitive(struct P2_primitive *prim)
{
    array__enqueue(registered_primitives_, (void *) prim);
    hash_table__add(primitives_dictionary_, (void *) prim->name, (void *) prim);
}



struct P2_primitive *P2_lookup_primitive(char *name)
{
    return (struct P2_primitive *) hash_table__lookup(primitives_dictionary_, (void *) name);
}



/*- end of file */
