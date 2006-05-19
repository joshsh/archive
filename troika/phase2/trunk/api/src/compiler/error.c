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

#include "Compiler-impl.h"


int
compiler__handle_parse_error( Compiler *c, char *msg )
{
    int ret = 0;

    if ( msg )
    {
        fprintf( stderr, "Error: %s\n\n", msg );
        free( msg );
    }

    else
        fprintf( stderr, "Parse error.\n\n" );

    memory_manager__collect_if_needed( environment__manager( c->env ) );

    return ret;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
