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
#include <stdio.h>  // fprintf
#include <string.h>  // strcat, strlen

/** \warning  No overflow checking. */
#define MAX_NAMELEN  100
#define MAX_PARAMS   100


int state__within_definition = 0;

p2_primitive *prim;

char prim__name[MAX_NAMELEN];
void *( *prim__cstub )( void** );
p2_type *prim__return_type;

int prim__parameters;
int prim__parameters;
char *prim__parameter_names[MAX_PARAMS];
char param_names_array[MAX_PARAMS * MAX_NAMELEN];
p2_type *prim__parameter_types[MAX_PARAMS];
char prim__transparency[MAX_PARAMS];


////////////////////////////////////////////////////////////////////////////////


void *p2_primdef__head(
    void *( *cstub )( void** ),
    char *name,
    char *return_type_id,
    int parameters )
{
    if ( state__within_definition )
    {
        fprintf( stderr,
            "Nested definition of p2_primitives is not allowed.\n" );
        return ( void* ) ( state__within_definition = 0 );
    }

    else if ( !name || !strlen( strcpy( prim__name, name )))
    {
        fprintf( stderr,
            "Missing function name for primitive.\n" );
        return 0;
    }

    else if ( !( prim__return_type = p2_type__lookup( return_type_id )) )
    {
        fprintf( stderr,
            "Return type \"%s\" of primitive \"%s\" is not a known type.\n",
            return_type_id, name );
        return 0;
    }

    else if ( !( prim__cstub = cstub ) )
    {
        fprintf( stderr,
            "Missing C stub for primitive \"%s\".\n", name );
        return 0;
    }

    else if (( prim__parameters = parameters ) < 0 )
    {
        fprintf( stderr,
            "Primitive \"%s\" cannot have %d parameters!\n", name, parameters );
        return 0;
    }

    else
    {
        int state__within_definition = 1;
        int prim__parameters = 0;
        return ( void* ) 1;
    }
}


void *p2_primdef__parameter(
    char *param_name,
    char *param_type_id,
    char param_transparency )
{
    if ( !state__within_definition )
    {
        fprintf( stderr,
            "Cannot define a parameter outside of a primitive definition.\n" );
        return 0;
    }

    else if ( !( prim__parameter_types[ prim__parameters ] = p2_type__lookup( param_type_id )))
    {
        fprintf( stderr,
           "Parameter type \"%s\" at index %d of primitive \"%s\" is not a known type.\n",
           param_type_id, prim__parameters, prim__name );
        return ( void* ) ( state__within_definition = 0 );
    }

    else if ( !param_name || !( prim__parameter_names[ prim__parameters ] = strcpy( param_names_array + ( prim__parameters * MAX_NAMELEN ), param_name )))
    {
        fprintf( stderr,
            "Missing parameter name for primitive \"s\".\n", prim__name );
        return ( void* ) ( state__within_definition = 0 );
    }

    else
    {
        prim__transparency[ prim__parameters ] = param_transparency;

        prim__parameters++;
        return ( void* ) 1;
    }
}


void *p2_primdef__tail( )
{
    if ( !state__within_definition )
    {
        fprintf( stderr,
            "Cannot define a parameter outside of a primitive definition.\n" );
        return 0;
    }

    else
    {
        state__within_definition = 0;

        return p2_primitive__new(
            prim__cstub,
            prim__name,
            prim__parameters,
            prim__parameter_types,
            prim__parameter_names,
            prim__transparency,
            prim__return_type ) ? ( void* ) 1 : 0;
    }
}

