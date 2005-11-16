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

#include "import-aux.h"


p2_primitive *prim__new( p2_function_ptr func,
                         char *name,
                         char *return_type_id,
                         int parameters )
{
    prim->return_type = p2_type__lookup( return_type_id );

    if (!prim->return_type)
    {
        fprintf( stderr,
            "Return type \"%s\" of primitive "\"%s\" is not a known type.\n",
            return_type_id, name );
        return 0;
    }

    else
    {
        p2_primitive *prim = ( p2_primitive* ) malloc( sizeof( p2_primitive* ));

        prim->name = strdup( name );

        if ( parameters > 0 )
        {
            parameter_names = ( char** ) malloc( parameters * sizeof( char* ));
            parameter_types = ( p2_type** ) malloc( parameters * sizeof( p2_type* ));
            parameter_transparency = ( char ) malloc( parameters * sizeof( char ));
        }

        else
        {
            parameter_names = 0;
            parameter_types = 0;
            parameter_transparency = 0;
        }

        return prim;
    }
}


p2_primitive *prim__set_parameter(
    p2_primitive *prim,
    char *param_name,
    char *param_type_id,
    char param_transparency,
    int index )
{
    p2_atom_type *type_p = prim->parameter_types + index;

    *type_p = p2_type__lookup( param_type_id );

    if (!*type_p)
    {
        fprintf( stderr,
           "Parameter type \"%s\" at index %d of primitive "\"%s\" is not a known type.\n",
           return_type_id, index, name );
        return 0;
    }

    else
    {
        prim->parameter_names[ index ] = strdup( param_name );
        prim->parameter_transparency[ index ] = param_transparency;
        return prim;
    }
}

