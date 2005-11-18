/**

\file  p2_type.h

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

#ifndef P2_TYPE_H
#define P2_TYPE_H


#include "p2_flags.h"
#include "p2_error.h"


/** Address is a unique 32-bit value to identify a data type. */
typedef struct _p2_type
{
    char *name;
    void ( *encoder )( void** );
    void ( *decoder )( void** );
    void ( *destructor )( void** );

} p2_type;


/** Type casting macro for first-class encoders. */
#define ENCODE_FORMAT    void (*)(void *, char *)

/** Type casting macro for first-class decoders. */
#define DECODE_FORMAT    void *(*)(char *)

/** Type casting macro for first-class destructors. */
#define DESTROY_FORMAT   void (*)(void *)

/** Type casting macro for first-class copy constructors. */
#define CLONE_FORMAT     void *(*)(void *)


/**  */
p2_error p2_type_init();

/**  */
p2_error p2_type_end();


/** Registers a new data type. */
p2_type *p2_type__new(

    char *name,
    void *( *encoder )( void** ),
    void *( *decoder )( void** ),
    void *( *destructor )( void** ));


/** Retrieves a type identifier by name. */
//p2_type *p2_type__lookup( char *name );

/**  */
//char *p2_type_name( p2_type *type );


/** Serializes a data item to a string. */
void p2_encode( void *p, p2_type *type, char *buffer );

/** Deserializes a data item from a string. */
void *p2_decode( p2_type *type, char *buffer );

/** Deallocates a data item. */
void p2_destroy( void *p, p2_type *type );

/** Creates a copy of a data item. */
//void *p2_clone( void *p, p2_type *type );


#endif  // P2_TYPE_H

