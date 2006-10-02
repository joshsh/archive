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


/** TRUE or FALSE. */
typedef int             boolean;

/** An alias which helps the importer distinguish a string value from a
    character value. */
typedef char            cstring;

/** Another string type, used for graphics. */
typedef char            xpm;


/******************************************************************************/


/** An instruction from a visitor function to a walk function. */
typedef int             ACTION;

/** An action which tells a walk function to continue visiting objects. */
#define CONTINUE        0

/** An action which tells a walk function to halt. */
#define BREAK           1

/** An action which tells a walk function to remove the current object from
    the collection being iterated over.  Not supported by all collection types. */
#define REMOVE          2


/******************************************************************************/


/** A function which compares two objects with respect to some ordering.  The
    return value is negative for "less than", positive for "greater than" and
    zero for "equal". */
typedef int             ( *Comparator )( const void *arg1, const void *arg2 );

/** Copy constructor. */
typedef void *          ( *Copy_Cons )( void *p );

/** Unary criterion. */
typedef boolean         ( *Criterion )( void *arg );

/** Binary criterion. */
typedef boolean         ( *Criterion2 )( void *arg1, void *arg2 );

/** Deserializer. */
typedef void *          ( *Decoder )( char *buffer );

/** Destructor. */
typedef void            ( *Destructor )( void *p );

/** Unary visitor. */
typedef ACTION          ( *Visitor )( void **refp );

/** Binary visitor. */
typedef ACTION          ( *Visitor2 )( void **refp1, void **refp2 );

/** Serializer. */
typedef void            ( *Encoder )( void *p, char *buffer );

/** A function which yields the number of objects in a collection. */
typedef unsigned int    ( *Size_Of )( const void *self );

/** Sorting function. */
typedef void            ( *Sort )( void *p, Comparator cmp );

/** Substitution function. */
typedef void *          ( *Substitution )( void *p );

/** A function which applies a visitor function to each object of a collection.
    It may halt when there are no more objects to visit, or when told to halt
    by the visitor function. */
typedef void            ( *Walker )( void *coll, Visitor f );

/** Any function.  Used for type-casting of callback functions. */
typedef void            ( *Generic_f )( void );


/* Type names *****************************************************************/


#define APPLY           Apply
#define ARRAY           Array
#define CHARACTER       char
#define COMBINATOR      Combinator
#define DOUBLE          double
#define INDIRECTION     Indirection
#define INTEGER         int
#define NAME            Name
#define NAMESPACE       Namespace
#define PRIMITIVE       Primitive
#define SET             Set
#define STRING          cstring
#define TERM            Term
#define TYPE            Type

#define ANY             any_type
#define VOID            Void

#define DATA_SET        data_set
#define OBJECT          object
#define TRIPLE          triple


/*
#define ENCODING__ROOT__XML__NAME               "data_set"

#define OBJECT__XML__NAME                       "object"
#define TRIPLES__XML__NAME                      "triple"

#define APPLY__XML__NAME                        "apply"
#define ARRAY__XML__NAME                        "list"
#define NAMESPACE__XML__NAME                    "table"
#define SET__XML__NAME                          "list"
#define TERM__XML__NAME                         "list"
*/


#endif  /* COMMON_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
