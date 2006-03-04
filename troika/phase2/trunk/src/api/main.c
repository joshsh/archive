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

#include "p2_compiler.h"


int main()
{
    int r = 0;

    p2_environment *env;
    p2_compiler *compiler;

    if ( !( env = p2_environment__new() ) )
        r = 1;

    else
    {
        if ( ( compiler = p2_compiler__new( env ) ) )
        {
            /* Print GPL notice. */
            printf( "\nPhase2 v%s, Copyright (C) 2005 Joshua Shinavier.\n", VERSION );
            printf( "The Phase2 programming environment comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions; type '!license;' for details.  Type '!quit;' to exit.\n\n" );

            p2_compiler__parse( compiler );

            p2_compiler__delete( compiler );
        }

        else
            r = 1;

        p2_environment__delete( env );
    }

    return r;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
