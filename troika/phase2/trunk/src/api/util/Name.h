/**

\file  p2_name.h

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

#ifndef P2_NAME_H
#define P2_NAME_H


#include "Array.h"


typedef Array p2_name;


#define name__peek( name )  ( char* ) array__peek( name )
#define name__pop( name )  ( char* ) array__pop( name )
#define name__push( name, s )  array__push( name, s )

p2_name *name__new();

void name__delete( p2_name *name );

void name__print( p2_name *name );


#endif  /* P2_NAME_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
