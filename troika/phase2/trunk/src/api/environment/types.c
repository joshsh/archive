/*******************************************************************************

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

*******************************************************************************/

#include "Environment-impl.h"


Object *
environment__register_type( Environment *env, Type *type )
{
    Object *o = object__new( env->type_t, type, OBJECT__IMMUTABLE );

    #if DEBUG__ENV
    printf( "[%#x] environment__register_type(%#x, %#x)\n",
        ( int ) o, ( int ) env, ( int ) type );
    #endif

    if ( !o )
        return 0;

    if ( !memory_manager__add( env->manager, o ) )
    {
        object__delete( o );
        return 0;
    }

    if ( namespace__add_simple( env->types->value, type->name, o ) )
        return 0;

    else
        return o;
}


Type_o *
environment__resolve_type( Environment *env, const char *name )
{
    Object *o;
    Type *type;

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

    #if DEBUG__ENVIRONMENT
    printf( "[%#x] environment__resolve_type(%#x, %s)\n",
        ( int ) o, ( int ) env, name );
    #endif

    return o;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
