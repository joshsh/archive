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

#include "p2_primitive.h"

#include "util/array.h"
#include "util/hash_table.h"

#include <stdio.h>  // sprintf
#include <string.h>  // memcpy, strdup



// P2_primitive type interface /////////////////////////////////////////////////



P2_primitive *P2_primitive__new(
    void *value,
    char *name,
    int parameters,
    P2_type *parameter_types,
    char **parameter_names,
    char *transparency,
    P2_type return_type)
{
    int i;

    P2_primitive *prim = (P2_primitive *) malloc(sizeof(P2_primitive));

    prim->value = value;
    prim->name = strdup(name);
    prim->parameters = parameters;

    prim->parameter_types = (P2_type *) malloc(parameters * sizeof(P2_type));
    memcpy(prim->parameter_types, parameter_types, parameters * sizeof(P2_type));

    if (!parameter_names)
        prim->parameter_names = 0;
    else
    {
        prim->parameter_names = (char **) malloc(parameters * sizeof(char *));
        for (i = 0; i < parameters; i++)
            prim->parameter_names[i] = strdup(parameter_names[i]);
    }

    if (!transparency)
        prim->transparency = 0;
    else
    {
        prim->transparency = (char *) malloc(parameters * sizeof(char));
        memcpy(prim->transparency, transparency, parameters * sizeof(char));
    }

    prim->return_type = return_type;

    #ifdef P2DEF_MANAGE_PRIMITIVES
        P2_primitive__retister(prim);
    #endif
}



void P2_primitive__delete(P2_primitive *prim)
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


#ifdef P2DEF_MANAGE_PRIMITIVES



P2_array *registered_primitives_;
P2_hash_table *primitives_dictionary_;



P2_error P2_primitive_init()
{
    registered_primitives_ = array__new(30, 2.0);
    primitives_dictionary_ = hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    return P2_SUCCESS;
}



P2_error P2_primitive_end()
{
    array__forall(registered_primitives_, (void (*)(void *)) P2_primitive__delete);
    array__delete(registered_primitives_);
    hash_table__delete(primitives_dictionary_);

    return P2_SUCCESS;
}



void P2_primitive__register(P2_primitive *prim)
{
    array__enqueue(registered_primitives_, (void *) prim);
    hash_table__add(primitives_dictionary_, (void *) prim->name, (void *) prim);
}



void P2_primitive__encode(P2_primitive *p, char *buffer)
{
    sprintf(buffer, p->name);
}



P2_primitive *P2_primitive__decode(char *buffer)
{
    return (P2_primitive *) hash_table__lookup(primitives_dictionary_, (void *) name);
}



#endif  // P2DEF_MANAGE_PRIMITIVES


