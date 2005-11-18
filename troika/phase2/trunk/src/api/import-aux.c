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

char prim__name[ MAX_NAMELEN ];
void *( *prim__cstub )( void** );
p2_type *prim__return_type;
enum function_marker prim__marker;

int prim__parameters;
char *prim__parameter_names[ MAX_PARAMS ];
char param_names_array[ MAX_PARAMS * MAX_NAMELEN ];
p2_type *prim__parameter_types[ MAX_PARAMS ];
char prim__transparency[ MAX_PARAMS ];


////////////////////////////////////////////////////////////////////////////////


static void *p2_cstub__free( void **args )
{
    free( args[ 0 ]);
    return 0;
}


static p2_type *get_type( char *name )
{
    p2_type *type = p2_type__lookup( name );

    if ( !type )
        // Create a new, empty type.
        type = p2_type__new( name, 0, 0, p2_cstub__free );

    return type;
}


static int is_encoder_type( p2_primitive *prim )
{
    return ( 1
        && ( prim->parameters == 2 )
        && ( !strcmp( prim->parameter_types[1]->name, "char" ))
        && ( !prim->return_type ));
}


static int is_decoder_type( p2_primitive *prim )
{
    return ( 1
        && ( prim->parameters == 1 )
        && ( !strcmp( prim->parameter_types[0]->name, "char" ))
        && ( prim->return_type ));
}


static int is_destructor_type( p2_primitive *prim )
{
    return ( 1
        && ( prim->parameters == 1 )
        && ( !prim->return_type ));
}


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

    else if ( !( prim__return_type = get_type( return_type_id )) )
    {
        fprintf( stderr,
            "Return type \"%s\" of primitive \"%s\" was not accepted.\n",
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
        state__within_definition = 1;
        prim__parameters = 0;
        prim__marker = 0;
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

    else if ( !( prim__parameter_types[ prim__parameters ] = get_type( param_type_id )))
    {
        fprintf( stderr,
           "Parameter type \"%s\" at index %d of primitive \"%s\" was not accepted.\n",
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


void *p2_primdef__tail( enum function_marker marker )
{
    p2_primitive *prim;
    prim__marker = marker;

    if ( !state__within_definition )
    {
        fprintf( stderr,
            "Cannot define a parameter outside of a primitive definition.\n" );
        return 0;
    }

    else if (
        !( prim = p2_primitive__new(
            prim__cstub,
            prim__name,
            prim__parameters,
            prim__parameter_types,
            prim__parameter_names,
            prim__transparency,
            prim__return_type )))
        return 0;

    else
    {
        switch ( prim__marker )
        {
            case encoder:

                // First valid encoder for the primitive's first parameter
                // type becomes that type's own encoder.
                if ( !prim->return_type->decoder
                  && is_encoder_type( prim ))
                    prim->return_type->decoder = prim;
                break;

            case decoder:

                // First valid decoder for the primitive's return type becomes
                // that type's own decoder.
                if ( !prim->return_type->decoder
                  && is_decoder_type( prim ))
                    prim->return_type->decoder = prim;
                break;

            case destructor:

                // First valid destructor for the primitive's first parameter
                // type becomes that type's own destructor.
                if ( !prim->return_type->decoder
                  && is_destructor_type( prim ))
                    prim->return_type->decoder = prim;
                break;

            default:

                // Do nothing.
        }

        state__within_definition = 0;
        return ( void* ) 1;
    }
}

