/*+

  p2.h

  Core library for the Phase2 programming language.
  
  Note: the prototypes for p2_error, p2_type and p2_primitive are just here for
  reference.

  last edited: 5/31/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_H
#define P2_H	1



#include "p2_debug.h"

typedef void * P2_error;
typedef void * P2_type;
typedef void * P2_function_ptr; //~
typedef void * P2_atom;

P2_type P2_error_type, P2_primitive_type, P2_id_type;

struct P2_identifier
{
	char *dataset_id;
	char *local_id;
};

#define P2_ID_NAME       "P2_identifier"



/** p2_error *//////////////////////////////////////////////////////////////////


/*
P2_error P2_register_error(char *name);

void P2_print_error(P2_error err, char *buffer);
*/

#include "p2_error.h"



/** p2_type *///////////////////////////////////////////////////////////////////


/*
P2_type P2_register_type(
    char *name,
	void (*encode)(P2_atom, char *),
    P2_atom (*decode)(char *),
    void (*destroy)(P2_atom),
    P2_atom (*clone)(P2_atom));

P2_type P2_type_lookup(char *name);

char *P2_type_name(P2_type type_index);

void P2_encode(P2_atom p, P2_type type_index, char *buffer);

P2_atom P2_decode(P2_type type_index, char *buffer);
*/

#include "p2_type.h"



/** p2_primitive *//////////////////////////////////////////////////////////////


/*
struct P2_primitive;

void P2_register_primitive(struct P2_primitive *prim);

struct P2_primitive *P2_lookup_primitive(char *name);
*/

#include "p2_primitive.h"



////////////////////////////////////////////////////////////////////////////////



P2_error P2_init();
P2_error P2_end();



#endif

/*- end of file */
