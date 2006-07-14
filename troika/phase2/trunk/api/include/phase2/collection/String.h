/**

\file  String.h

\brief Provides an expanding string class.

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

#ifndef STRING_H
#define STRING_H

/*#include <stdarg.h>*/

/*#include <common.h>*/

/** An expanding string of characters. */
typedef struct String String;


/* Constructors and destructor ************************************************/

/** Constructor. */
extern String *
string__new( unsigned int buffer_size );

/** Copy constructor. */
extern String *
string__copy( String *s );

/** Destructor. */
extern void
string__delete( String *s );


/******************************************************************************/

/** \return  the length of the String. */
extern unsigned int
string__size( const String *a );

/** \return  the contents of the string as a character array
    \note  You must call free() on the returned string when it is no longer
    needed. */
extern char *
string__to_cstring( const String *s );

/** Suffixes a character array to the String.
    \return  the appended string, or NULL if unsuccessful */
extern char *
string__append( String *s, char *cstring );

/**
    \return  FALSE if successful, TRUE if unsuccessful */
/*
extern boolean
string__sprintf( String *s, unsigned int minlen, ... );
*/


/* Miscellaneous **************************************************************/

/** Empties the String. */
extern String *
string__clear( String *s );


#endif  /* STRING_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
