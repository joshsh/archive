/**

\file  p2.h

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

#ifndef P2_H
#define P2_H


#include "p2_flags.h"


typedef enum _boolean
{
    boolean__false = 0,
    boolean__true

} boolean;


typedef void *  ( *allocator )( void );
typedef int     ( *comparator )( void *arg1, void *arg2 );
typedef void *  ( *copy_cons )( void *p );
typedef boolean ( *criterion )( void *arg );
typedef boolean ( *criterion2 )( void *arg1, void *arg2 );
typedef void *  ( *decoder )( char *buffer );
typedef void    ( *destructor )( void *p );
typedef void    ( *encoder )( void *p, char *buffer );

typedef void    ( *sort_f )( void *p, comparator cmp );
typedef void *  ( *substitution_f )( void *p, void *state );
typedef void    ( *void_f )( void *p );


/* Procedures (functions with state) ******************************************/


typedef enum _p2_procedure__effect
{
    p2_procedure__effect__continue = 0,

    p2_procedure__effect__break,
    p2_procedure__effect__remove

} p2_procedure__effect;


typedef p2_procedure__effect ( *procedure )( void *addr, void *state );


typedef struct _p2_procedure
{
    procedure execute;

    /* A mutable data field which is provided to the procedure as an argument,
       and preserved between invocations. */
    void *state;

} p2_procedure;


#define p2_procedure__execute( p, addr )  p->execute( addr, p->state )

typedef void    ( *distributor )( void* p, p2_procedure *a );


#endif  /* P2_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
