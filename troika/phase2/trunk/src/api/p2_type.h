/**

\file  p2_type.h

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

#ifndef P2_TYPE_H
#define P2_TYPE_H


#include "p2_flags.h"


/******************************************************************************/


typedef enum _p2_action__effect
{
    p2_action__effect__continue = 0,

    p2_action__effect__break,
    p2_action__effect__remove

} p2_action__effect;


typedef p2_action__effect ( *action )( void *addr, void *context );


typedef struct _p2_action
{
    action execute;

    /* A mutable data field which is provided to the action as an argument,
       and preserved between invocations. */
    void *context;

} p2_action;


#define p2_action__apply( a, addr )  a->execute( addr, a->context )


typedef void *( *decoder )( char *buffer );
typedef void ( *destructor )( void *p );
typedef void ( *distributor )( void* p, p2_action *a );
typedef void ( *encoder )( void *p, char *buffer );


/******************************************************************************/


/** */
typedef struct _p2_type
{
    /** */
    char *name;

    /** Copy constructor. */
    void *( *clone )( void* );

    /** Deserializer. */
    void *( *decode )( char* );

    /** Destructor. */
    void ( *destroy )( void* );

    /** Serializer. */
    void ( *encode )( void*, char* );

    /** "Element exists" callback distributor. */
    void *( *exists )( void*, void *(*)( void* ) );

    /** "For all elements" callback distributor. */
    void *( *for_all )( void*, void *(*)( void* ) );

    distributor distribute;

} p2_type;


#define CLONE_T     void *(*)(void *)
#define DECODE_T    void *(*)(char *)
#define DESTROY_T   void (*)(void *)
#define ENCODE_T    void (*)(void *, char *)
#define EXISTS_T    void *(*)(void *, void *(*)(void*))
#define FOR_ALL_T   void *(*)(void *, void *(*)(void*))


/** Constructor.
    \note  The type assumes ownership if its name argument. */
p2_type *p2_type__new
(
    char *name,
    void *( *clone )( void* ),
    void *( *decode )( char* ),
    void ( *destroy )( void* ),
    void ( *encode )( void*, char* ),
    void *( *exists )( void*, void *(*)( void* ) ),
    void *( *for_all )( void*, void *(*)( void* ) ),
    distributor distribute
);

/** Destructor. */
void p2_type__delete( p2_type *type );


#endif  /* P2_TYPE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
