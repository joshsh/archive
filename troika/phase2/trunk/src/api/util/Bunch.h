/**
    \file  Bunch.h

    \brief  A utility to keep track of large, unordered collections of pointers.

    Once added to a bunch, items cannot be retrieved individually, but are
    handled en masse via the "for all" "exists" and "exclude if" functions, which
    mimic set operations.

    \author  Joshua Shinavier   \n
             parcour@gmail.com  \n
             +1 509 570-6990    \n */

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

#ifndef BUNCH_H
#define BUNCH_H

#include <util/Array.h>


typedef struct Bunch Bunch;


/******************************************************************************/

/** Constructor. */
extern Bunch *
bunch__new( unsigned int block_size );

/** Copy constructor. */
extern Bunch *
bunch__copy( Bunch *b );

/** Destructor. */
extern void
bunch__delete( Bunch *b );


/******************************************************************************/

/** How big of a bunch is it? */
extern unsigned int
bunch__size( Bunch *b );


/******************************************************************************/

/** Adds a single item to the bunch. */
extern void *
bunch__add( Bunch *b, void *p );

/** Adds the contents of one bunch to another. */
extern void
bunch__add_all( Bunch *dest, Bunch *src );

/** Removes a single item from the bunch. */
extern void *
bunch__remove( Bunch *b );


/******************************************************************************/

extern void
bunch__distribute( Bunch *b, p2_procedure *p );


/******************************************************************************/

extern Type *
bunch__type( const char *name, int flags );


#endif  /* BUNCH_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */

