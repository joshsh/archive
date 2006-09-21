/**

\file  Closure.h

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

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

#ifndef CLOSURE_H
#define CLOSURE_H


#include <phase2/common.h>


typedef void *( *procedure )( void *p, void *state );


typedef struct Closure Closure;

/** \param c  a Closure of function type "boolean (*)(void**)" */
typedef void    ( *Distributor )( void *p, Closure *c );


/******************************************************************************/

extern Closure *
closure__new( procedure execute, void *state );

extern void
closure__delete( Closure *c );

extern void *
closure__apply( Closure *c, void *arg );

/** \param c  Closure of function type "boolean (*)(void*)"
    \return  Closure c2 such that c2 p = c *p.  It has function type
    "boolean (*)(void**)". */
extern Closure *
closure__cw_dereference( Closure *c );


#endif  /* CLOSURE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
