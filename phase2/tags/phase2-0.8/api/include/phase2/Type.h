/**

\file  Type.h

\brief  Provides a class which defines a data type.

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

#ifndef TYPE_H
#define TYPE_H


#include <phase2/common.h>
#include <phase2/String.h>


/*
typedef void ( *Encoder_Safe )(
    void *p,
    void ( *print )( const char *s ) );
*/

/** A serializer function which is immune to buffer overflow. */
typedef void            ( *Encoder_Safe )( void *p, String *buffer );


enum Type__Flags
{
    /** Indicates that this is a collection of first-class values. */
    TYPE__IS_OBJ_COLL         = 0x1,

    /** Indicates that child objects are owned, not shared. */
    TYPE__OWNS_DESCENDANTS    = 0x2
};

/** A structure representing the data type of an Object, and containing
    type-specific member functions. */
typedef struct Type Type;


#define any_type    ( Type* ) 42


/** Constructor. */
extern Type *
type__new( const char *name, int flags );


/** Destructor. */
extern void
type__delete( Type *type );


/******************************************************************************/

/** \return  a new instance of the type of types */
extern Type *
type__create_type( const char *name );

/** \param arg  the type argument for collection types */
extern void
type__set_type_arg( Type *type, Type *arg );


#endif  /* TYPE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
