/**

\file  Void.h

\brief  Defines the trivial Void data type.

\note  It is assumed that the implementation will not call Void__new so
       many times as to use up every below-range address.

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

#ifndef VOID_H
#define VOID_H

#include <common.h>


/** A Void item is a pointer to an out-of-range memory location, which serves
    as a unique identifier for the item but does not contain any meaningful
    data. */
typedef unsigned int Void;


/** The constructor merely returns an unused out-of-range address. */
Void *
void__new();


#endif  /* VOID_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
