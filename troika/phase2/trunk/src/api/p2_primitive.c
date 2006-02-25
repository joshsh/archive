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

#include "p2_primitive.h"


p2_primitive *p2_primitive__new
(
    void *( *cstub )( void ** ),
    char *name,
    int n_params,
    p2_type **parameter_types,
    char **parameter_names,
    char *transparency,
    p2_type *return_type
)
{
    #if DEBUG__SAFE
        if ( !cstub || !name || !parameter_types )
        {
            PRINTERR( "p2_primitive__new: null argument(s)" );
            return 0;
        }
    #endif

    p2_primitive *prim = new( p2_primitive );

    if ( !prim )
        return 0;

    prim->cstub = cstub;
    prim->name = name;
    prim->n_params = n_params;
    prim->parameter_types = parameter_types;
    prim->parameter_names = parameter_names;
    prim->transparency = transparency;
    prim->return_type = return_type;
/*

    #ifdef P2FLAGS__MANAGE_PRIMITIVES
        p2_primitive__register(prim);
    #endif
*/

    return prim;
}


void p2_primitive__delete( p2_primitive *prim )
{
    #if DEBUG__SAFE
    if ( !prim )
    {
        PRINTERR( "p2_primitive__delete: null primitive" );
        return;
    }
    #endif

    free( prim->name );
    free( prim->parameter_types );
    if ( prim->transparency )
        free( prim->transparency );
    if ( prim->parameter_names )
        free( prim->parameter_names );

    free(prim);
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
