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

#include "Environment-impl.h"


Object *
environment__register_type( Environment *env, Type *type )
{
    Object *o;

    if ( DEBUG__SAFE && ( !env || !type ) )
        abort();

    o = manager__object( env->manager, env->type_t, type, OBJECT__IMMUTABLE );

    if ( !o )
        return 0;

    return namespace__add_simple( env->types->value, type->name, o );
}


OBJ( TYPE ) *
environment__resolve_type( Environment *env, const char *name, boolean create_if_absent )
{
    Object *o;
    Type *type;

    if ( DEBUG__SAFE && ( !env || !name ) )
        abort();

    if ( !( o = namespace__lookup_simple( ( Namespace* ) env->types->value, name ) ) )
    {
        /* If not found, create the type and hope for the best. */
        if ( create_if_absent )
        {
            if ( ( type = type__new( name, 0 ) ) )
            {
                /* NOTE: all object collection types are registered explicitly. */
                if ( !( o = environment__register_type( env, type ) ) )
                {
                    type__free( type );
                }
            }
        }
    }

    return o;
}


Type *
environment__create_type( Environment *e, const char *name, int flags )
{
    Type *t;
    Object *o;

    if ( DEBUG__SAFE && ( !e || !name ) )
        abort();

    if ( namespace__lookup_simple( e->types->value, name ) )
    {
        ERROR( "Type %s already exists", name );
        return 0;
    }

    t = type__new( name, flags );
    if ( !t )
        return 0;

    /* The "Type" type is a special case.  It MUST be created before all other
       types. */
    if ( !strcmp( name, NAMEOF( TYPE ) ) )
        e->type_t = t;

    o = manager__object( e->manager, e->type_t, t, OBJECT__IMMUTABLE );

    if ( !o )
        return 0;

    if ( !namespace__add_simple( e->types->value, name, o ) )
        return 0;

    else
        return t;
}


Object *
environment__create_object( Environment *e, Type *type, void *value )
{
    if ( DEBUG__SAFE && ( !e || !type || !value ) )
        abort();

    return manager__object( e->manager, type, value, NOFLAGS );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
