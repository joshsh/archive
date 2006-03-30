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

#include <Compiler.h>
#include <serial.h>


/** Instantiates an Environment and attaches a Compiler to interact with the
    command line. */
int main( int argc, char *argv[] )
{
    int status = EXIT_SUCCESS;

    Environment *env;
    Compiler *compiler;

    if ( !( env = environment__new() ) )
        status = EXIT_FAILURE;

    else
    {
        if ( ( compiler = compiler__new( env ) ) )
        {
            if ( argc > 1 )
            {
                printf( "Loading namespace from file...\n" );
                compiler__deserialize( compiler, argv[1] );
            }

            /* Print GPL notice. */
            printf( "\nPhase2 v%s, Copyright (C) 2005-2006 Joshua Shinavier.\n", VERSION );
            printf( "The Phase2 programming environment comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions; type '_license;' for details.  Type '_quit;' to exit.\n\n" );

            compiler__parse( compiler );

            compiler__delete( compiler );
        }

        else
            status = EXIT_FAILURE;

        environment__delete( env );
    }

    /* See: http://www.jetcafe.org/~jim/c-style.html#Expressions */
    exit( status );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
