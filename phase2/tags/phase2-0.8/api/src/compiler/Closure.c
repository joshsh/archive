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

#include <phase2/Closure.h>
#include "settings.h"


struct Closure
{
    procedure execute;

    /* A mutable data field which is provided to the procedure as an argument,
       and preserved between invocations. */
    void *state;
};


/******************************************************************************/


Closure *
closure__new( procedure execute, void *state )
{
    Closure *c;

    if ( ( c = NEW( Closure ) ) )
    {
        c->execute = execute;
        c->state = state;
    }

    else
        ERROR( "closure__new: allocation failure" );

    return c;
}


void
closure__delete( Closure *c )
{
    if ( DEBUG__SAFE && !c )
        ABORT;

    free( c );
}


void *
closure__apply( Closure *c, void *arg )
{
    if ( DEBUG__SAFE && ( !c || !arg ) )
        ABORT;

    return c->execute( arg, c->state );
}


/******************************************************************************/


static void *
dereference( void **refp, Closure *c )
{
    return closure__apply( c, *refp );
}


Closure *
closure__cw_dereference( Closure *c )
{
    Closure *c2 = NEW( Closure );

    if ( c2 )
    {
        c2->execute = ( procedure ) dereference;
        c2->state = c;
    }

    return c2;
}


/*
static boolean
returnfalse( void *p, Closure *c )
{
    closure__apply( c, p );
    return FALSE;
}


Closure *
closure__cw_return_false( Closure *c )
{
    Closure *c2 = NEW( Closure );

    if ( c2 )
    {
        c2->execute = ( procedure ) returnfalse;
        c2->state = &c;
    }

    return c2;
}
*/


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
