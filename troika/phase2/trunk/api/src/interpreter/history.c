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

#include "Interpreter-impl.h"


#if HAVE_LIBREADLINE
#    include <readline/history.h>
static boolean history_initialized = FALSE;
static void
enable_history()
{
    if ( !history_initialized )
    {
        using_history();
        history_initialized = TRUE;
    }
}
#endif


void
interpreter__add_to_history( const char *s )
{
#if HAVE_LIBREADLINE
    char *t, *cur;

    enable_history();

    if ( s && *s )
    {
        t = STRDUP( s );
        cur = t + strlen( t ) - 1;
        while ( *cur <= ' ' )
            cur--;
        cur++;
        *cur = '\0';
        cur = t;
        while ( '\n' == *cur )
            cur++;

        add_history( cur );

        free( t );
    }
#endif
}


void
interpreter__print_history()
{
#if HAVE_LIBREADLINE
    register HIST_ENTRY **list;
    register int i;

    enable_history();

    list = history_list();
    if ( list )
    {
        for ( i = 0; list[i]; i++ )
/*
            PRINT( "%d: %s\n", i + history_base, list[i]->line );
*/
            PRINT( "%s\n", list[i]->line );
    }
#endif
}


void
interpreter__write_history( const char *file )
{
#if HAVE_LIBREADLINE
    enable_history();

    if ( file && *file )
        write_history( file );
    else
        ERROR( "null or empty file name" );
#endif
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
