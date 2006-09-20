/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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

#include "Primitive-impl.h"
    #include "../type/Type-impl.h"


void
primitive__delete( Primitive *prim )
{
    unsigned int i;

    if ( DEBUG__SAFE && !prim )
        ABORT;

    free( prim->name );

    for ( i = 0; i < prim->arity; i++ )
        if ( prim->parameters[i].name )
            free( prim->parameters[i].name );

    free( prim->parameters );

    free( prim );
}


/******************************************************************************/


char *
primitive__name( Primitive *p )
{
    return STRDUP( p->name );
}


unsigned int
primitive__arity( Primitive *p )
{
    return p->arity;
}


Type *
primitive__return_type( Primitive *p )
{
    return p->return_type;
}


static boolean
out_of_bounds( unsigned int arity, unsigned int i )
{
    if ( i >= arity )
    {
        ERROR( "parameter index out of bounds" );
        return TRUE;
    }

    else
        return FALSE;
}


char *
primitive__parameter_name( Primitive *p, unsigned int i )
{
    if ( out_of_bounds( p->arity, i ) )
        return 0;

    return STRDUP( p->parameters[i].name );
}


Type *
primitive__parameter_type( Primitive *p, unsigned int i )
{
    if ( out_of_bounds( p->arity, i ) )
        return 0;

    return p->parameters[i].type;
}


boolean
primitive__parameter_reftrans( Primitive *p, unsigned int i )
{
    if ( out_of_bounds( p->arity, i ) )
        return 0;

    return p->parameters[i].transparent;
}


/******************************************************************************/


void
primitive__encode( Primitive *prim, char *buffer )
{
    unsigned int i;
    Parameter p;
    char *name;

    char *generic_name = "any";

    char *type_name( Type *t )
    {
        if ( t == any_type )
            return generic_name;
        else
            return t->name;
    }

    for ( i = 0; i < prim->arity; i++ )
    {
        if ( i )
        {
            sprintf( buffer, ", " );
            buffer += 2;
        }

        p = prim->parameters[i];
        sprintf( buffer, "<%s>", type_name( p.type ) );
        buffer += strlen( buffer );

        name = p.name;
        if ( name && *name )
        {
            sprintf( buffer, " " );
            buffer++;

            /* Prepend a '!' to the parameter name if the primitive is not
               referentially transparent with respect to the parameter. */
            if ( !p.transparent )
            {
                sprintf( buffer, "!" );
                buffer++;
            }

            sprintf( buffer, "%s", name );
            buffer += strlen( buffer );
        }
    }

    sprintf( buffer, " --> <%s>", type_name( prim->return_type ) );

    /*sprintf( buffer, prim->name );*/
}


Type *
primitive__create_type( const char *name )
{
    Type *t = type__new( name, NOPROPS );

    if ( t )
    {
        t->destroy = ( Destructor ) primitive__delete;
        t->encode = ( Encoder ) primitive__encode;
    }

    return t;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
