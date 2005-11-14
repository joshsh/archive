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

#include "util/p2_array.h"
#include "util/p2_hash_table.h"

#include <stdio.h>  // sprintf
#include <string.h>  // memcpy, strdup



// p2_primitive type interface /////////////////////////////////////////////////



p2_primitive *p2_primitive__new(
    void *value,
    char *name,
    int parameters,
    p2_type *parameter_types,
    char **parameter_names,
    char *transparency,
    p2_type return_type)
{
    int i;

    p2_primitive *prim = (p2_primitive *) malloc(sizeof(p2_primitive));

    prim->value = value;
    prim->name = strdup(name);
    prim->parameters = parameters;

    prim->parameter_types = (p2_type *) malloc(parameters * sizeof(p2_type));
    memcpy(prim->parameter_types, parameter_types, parameters * sizeof(p2_type));

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

    #ifdef P2FLAGS__MANAGE_PRIMITIVES
        p2_primitive__retister(prim);
    #endif
}



void p2_primitive__delete(p2_primitive *prim)
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


#ifdef P2FLAGS__MANAGE_PRIMITIVES



p2_array *registered_primitives_;
p2_hash_table *primitives_dictionary_;



p2_error p2_primitive_init()
{
    registered_primitives_ = p2_array__new(30, 2.0);
    primitives_dictionary_ = p2_hash_table__new(60, 2.0, 2.0, STRING_DEFAULTS);

    return P2_SUCCESS;
}



p2_error p2_primitive_end()
{
    p2_array__forall(registered_primitives_, (void (*)(void *)) p2_primitive__delete);
    p2_array__delete(registered_primitives_);
    p2_hash_table__delete(primitives_dictionary_);

    return P2_SUCCESS;
}



void p2_primitive__register(p2_primitive *prim)
{
    p2_array__enqueue(registered_primitives_, (void *) prim);
    p2_hash_table__add(primitives_dictionary_, (void *) prim->name, (void *) prim);
}



void p2_primitive__encode(p2_primitive *p, char *buffer)
{
    sprintf(buffer, p->name);
}



p2_primitive *p2_primitive__decode(char *buffer)
{
    return (p2_primitive *) p2_hash_table__lookup(primitives_dictionary_, (void *) name);
}



#endif  // P2FLAGS__MANAGE_PRIMITIVES


