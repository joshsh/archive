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


#include "flags.h"


#include <stdlib.h>  /* malloc */
#include <string.h>  /* strlen, strcpy */

#define STRDUP( x )  strcpy( ( char* ) malloc( 1 + strlen( x ) ), x )
#define new( type )  ( type* ) malloc( sizeof( type ) )


/******************************************************************************/


typedef char cstring;

typedef enum _boolean
{
    boolean__false = 0,
    boolean__true

} boolean;


typedef void *  ( *allocator )( void );
typedef int     ( *comparator )( const void *arg1, const void *arg2 );
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


typedef struct _p2_procedure
{
    procedure execute;

    /* A mutable data field which is provided to the procedure as an argument,
       and preserved between invocations. */
    void *state;

} p2_procedure;


#define p2_procedure__execute( p, data )  p->execute( data, p->state )

typedef void    ( *distributor )( void *data, p2_procedure *a );


#define VERSION "0.5.0"


/* Errors *********************************************************************/


#include <stdio.h>  /* fprintf */

#define ERROR( msg ) \
{ \
    fprintf( stderr, "Error: %s.\n", msg ); \
    fflush( stdout ); \
}

/* Useful, but not ANSI C.
#define ERROR( ... ) \
{ \
    fprintf( stderr, "Error: " ); \
    fprintf( stderr, __VA_ARGS__ ); \
    fprintf( stderr, ".\n" ); \
    fflush( stdout ); \
}
*/

#define WARNING( msg ) \
{ \
    fprintf( stderr, "Warning: %s.\n", msg ); \
    fflush( stdout ); \
}


#endif  /* P2_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
