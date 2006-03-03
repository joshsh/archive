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


#include "p2_array.h"


typedef p2_array p2_name;


#define p2_name__peek( name )  ( char* ) p2_array__peek( name )
#define p2_name__pop( name )  ( char* ) p2_array__pop( name )
#define p2_name__push( name, s )  p2_array__push( name, s )


void p2_name__print( p2_name *name );


#endif  /* P2_NAME_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
