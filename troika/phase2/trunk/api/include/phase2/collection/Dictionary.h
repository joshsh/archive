/**

\file  Dictionary.h

\brief  Provides a lexical lookup table class.

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

#ifndef DICTIONARY_H
#define DICTIONARY_H


#include <collection/Hash_Table.h>
#include <collection/Array.h>


/** An object which binds string keys to arbitrary targets. */
typedef Hash_Table Dictionary;


/** \return  a new dictionary */
extern Dictionary *
dictionary__new( void );

/** Destructor. */
extern void
dictionary__delete( Dictionary *d );


/** Adds a key/target pair to the dictionary. */
extern void *
dictionary__add( Dictionary *d, char *key, void *target );

/** \return  the target value bound to the given key by the dictionary */
extern void *
dictionary__lookup( Dictionary *d, char *key );

/** \return  the key (if any) which maps to the given target in the
    dictionary */
extern char *
dictionary__reverse_lookup( Dictionary *d, const void *target );

/** Removes the pair with the given key. */
extern void *
dictionary__remove( Dictionary *d, char *key );


/** Adds the contents of one dictionary to another. */
extern void
dictionary__add_all( Dictionary *dest, Dictionary *src );


/** Applies a visitor function to each target object in the dictionary. */
extern void
dictionary__walk( Dictionary *d, Visitor f );


/** \return  an array of all key values in the dictionary */
extern Array *
dictionary__keys( Dictionary *d );


#endif  /* DICTIONARY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
