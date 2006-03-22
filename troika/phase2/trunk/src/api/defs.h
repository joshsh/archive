/**

\file  defs.h

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

#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>  /* malloc */
#include <string.h>  /* strlen, strcpy */


#include "settings.h"


/******************************************************************************/


#define VERSION "0.6.0"


#define STRDUP( x )  strcpy( malloc( 1 + strlen( x ) ), (x) )
#define new( type )  malloc( sizeof (type) )


/******************************************************************************/


typedef char cstring;

enum boolean
{
    boolean__false = 0,
    boolean__true
};

typedef enum boolean boolean;


/* Procedures (functions with state) ******************************************/


typedef enum _p2_action__type
{
    p2_action__type__noop = 0,

    p2_action__type__break,
    p2_action__type__remove,
    p2_action__type__replace

} p2_action__type;


typedef struct _p2_action
{
    p2_action__type type;

    /* For p2_action__type__replace: the replacement value. */
    void *value;

} p2_action;


typedef p2_action *( *procedure )( void *data, void *state );


typedef struct Closure Closure;

struct Closure
{
    procedure execute;

    /* A mutable data field which is provided to the procedure as an argument,
       and preserved between invocations. */
    void *state;
};


/* ~ capitalization */
#define Closure__execute( p, data )  (p)->execute( (data), (p)->state )


/******************************************************************************/


typedef void *  ( *Allocator )( void );
typedef int     ( *Comparator )( const void *arg1, const void *arg2 );
typedef void *  ( *Copy_Cons )( void *p );
typedef boolean ( *Criterion )( void *arg );
typedef boolean ( *Criterion2 )( void *arg1, void *arg2 );
typedef void *  ( *Decoder )( char *buffer );
typedef void    ( *Destructor )( void *p );
typedef void    ( *Distributor )( void *data, Closure *a );
typedef void    ( *Encoder )( void *p, char *buffer );

typedef void    ( *Sort )( void *p, Comparator cmp );
typedef void *  ( *Substitution )( void *p, void *state );
typedef void    ( *Void_f )( void *p );

typedef void    ( *Generic_f )( void );


/* Errors *********************************************************************/


#include <stdio.h>  /* fprintf */

#define FFLUSH  { fflush( stdout ); fflush( stderr ); }

#define ERROR( msg )                                                        \
{                                                                           \
    fprintf( stderr, "Error: %s.\n", (msg) );                               \
    fflush( stdout );                                                       \
}

/* Useful, but not ANSI C.
#define ERROR( ... )                                                        \
{                                                                           \
    fprintf( stderr, "Error: " );                                           \
    fprintf( stderr, __VA_ARGS__ );                                         \
    fprintf( stderr, ".\n" );                                               \
    fflush( stdout );                                                       \
}
*/

#define WARNING( msg )                                                      \
{                                                                           \
    fprintf( stderr, "Warning: %s.\n", (msg) );                             \
    fflush( stdout );                                                       \
}


#endif  /* DEFS_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
