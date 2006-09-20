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

#if USE_NCURSES
#include <ncurses.h>
#endif

#include "Interpreter-impl.h"


int
interpreter__handle_parse_error( Interpreter *c, const char *msg )
{
    int ret = 0;

#if USE_NCURSES
    /*initscr();*/
    if( has_colors() == FALSE )
    {
        endwin();
        PRINT( "Your terminal does not support color\n" );
        exit(1);
    }
    start_color();
    init_pair( 1, COLOR_RED, COLOR_WHITE );
    attron( COLOR_PAIR(1) );
#endif

    if ( msg )
        ERROR( "%s\n", msg );

    else
        ERROR( "parse error\n" );

#if USE_NCURSES
    attroff(COLOR_PAIR(1));
    /*endwin();*/
#endif

    manager__collect( environment__manager( c->env ), FALSE, FALSE );

    return ret;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
