/**

\file p2_void.h

\brief  Defines the trivial p2_void data type.

\note  It is assumed that the implementation will not call p2_void__new so
       many times as to use up every below-range address.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_VOID_H
#define P2_VOID_H


/** A p2_void item is a pointer to an out-of-range memory location, which serves
    as a unique identifier for the item but does not contain any meaningful
    data. */
typedef void p2_void;


/** The constructor merely returns an unused out-of-range address. */
p2_void *p2_void__new(char *dataset_id, char *local_id);


#endif  // P2_VOID_H

