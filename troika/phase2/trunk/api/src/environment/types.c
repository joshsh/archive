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

    o = memory_manager__object( env->manager, env->type_t, type, OBJECT__IMMUTABLE );

    if ( !o )
        return 0;

    return namespace__add_simple( env->types->value, type->name, o );
}


Type_o *
environment__resolve_type( Environment *env, const char *name )
{
    Object *o;
    Type *type;

    if ( DEBUG__SAFE && ( !env || !name ) )
        abort();

    if ( !( o = namespace__lookup_simple( ( Namespace* ) env->types->value, name ) ) )
    {
        /* If not found, create the type and hope for the best. */
        if ( ( type = type__new( name, 0 ) ) )
        {
            /* Note: all object collection types are registered explicitly. */
            if ( !( o = environment__register_type( env, type ) ) )
            {
                type__delete( type );
            }
        }
    }

    return o;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
