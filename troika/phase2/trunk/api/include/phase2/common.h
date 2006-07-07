/**

\file  common.h

\brief  Data and callback type aliases.

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

#ifndef COMMON_H
#define COMMON_H


typedef int boolean;

typedef char cstring;
typedef char xpm;


typedef int ACTION;

typedef void *( *Dist_f )( void **refp );

#define CONTINUE            0
#define BREAK               ( void* ) 1
#define REMOVE              ( void* ) 2

typedef void *          ( *Allocator )( void );
typedef int             ( *Comparator )( const void *arg1, const void *arg2 );
typedef void *          ( *Copy_Cons )( void *p );
typedef boolean         ( *Criterion )( void *arg );
typedef boolean         ( *Criterion2 )( void *arg1, void *arg2 );
typedef void *          ( *Decoder )( char *buffer );
typedef void            ( *Destructor )( void *p );
typedef void            ( *Encoder )( void *p, char *buffer );
typedef unsigned int    ( *Size_Of )( const void *self );
typedef void            ( *Sort )( void *p, Comparator cmp );
typedef void *          ( *Substitution )( void *p );
typedef void            ( *Void_f )( void *p );
typedef void            ( *Walker )( void *coll, Dist_f f );

typedef void            ( *Generic_f )( void );


#define NOPROPS 0


#endif  /* COMMON_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
