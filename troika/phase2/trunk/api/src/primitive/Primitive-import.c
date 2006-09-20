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

#include <Primitive-import.h>
#include "Primitive-impl.h"


static Type *
resolve_type( Environment *env, const char *name )
{
    Object *o;

    if ( !strcmp( name, NAMEOF( ANY ) ) )
        return any_type;
    else
    {
        if ( ( o = environment__resolve_type( env, name ) ) )
            return object__value( o );

        else
        {
            ERROR( "resolve_type: type not found" );
            return 0;
        }
    }
}


Primitive *
primitive__new(
    Environment *env,
    const char *return_type,
    const char *name,
    void *( *cstub )( void** ),
    int arity )
{
    int i;
    Primitive *p;

    if ( DEBUG__SAFE && ( !name || !cstub || !return_type ) )
        ABORT;

    if ( !( p = NEW( Primitive ) ) )
    {
        if ( DEBUG__SAFE )
            ERROR( "primitive__new: allocation failed" );

        return 0;
    }

    p->return_type = resolve_type( env, return_type );

    if ( DEBUG__SAFE && !p->return_type )
    {
        ERROR( "primitive__new: unknown type" );
        primitive__delete( p );
        return 0;
    }

    p->name = STRDUP( name );
    p->cstub = cstub;

    p->arity = arity;

    if ( DEBUG__SAFE && arity <= 0 )
    {
        ERROR( "primitive__new: bad arity" );
        free( p->name );
        free( p );
        return 0; 
    }

    p->parameters = malloc( arity * sizeof( Parameter ) );
    for ( i = 0; i < arity ; i++ )
    {
        /* Mark the parameter as "not yet defined". */
        p->parameters[i].type = 0;
        p->parameters[i].name = 0;
    }

    return p;
}


Primitive *
primitive__add_param(
    Environment *env,
    Primitive *p,
    char *type,
    char *name,
    boolean transparent )
{
    unsigned int i;
    Parameter param;

    if ( DEBUG__SAFE && ( !p || !name || !type ) )
        ABORT;

    param.type = resolve_type( env, type );

    param.name = STRDUP( name );
    param.transparent = transparent;

    for ( i = 0; i < p->arity; i++ )
        if ( !p->parameters[i].type )
            break;

    if ( DEBUG__SAFE && i == p->arity )
    {
        ERROR( "primitive__add_param: too many parameters" );
        primitive__delete( p );
        return 0;
    }

    p->parameters[i] = param;

    return p;
}


Primitive *
primitive__register(
    Environment *env,
    Primitive *p,
    int flags,
    void ( *src_f ) ( void ) )
{
    if ( DEBUG__SAFE && ( !env || !p ) )
        ABORT;

    environment__register_primitive( env, p, flags, src_f );

    return p;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
