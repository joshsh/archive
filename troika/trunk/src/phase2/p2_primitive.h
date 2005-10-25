/*+

  p2_primitive.h

  last edited: 6/4/05

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

#ifndef P2_PRIMITIVE_H
#define P2_PRIMITIVE_H



#include "p2_type.h"

#define P2_PRIMITIVE_NAME    "PrimitiveReference"



P2_type P2_primitive_type;

struct P2_primitive_
{
  char *name;

  // Physical location of the C function.
  void *value;

  // Number of input parameters.
  int parameters;

  // An array of length {parameters} containing the data type of all input
  // parameters in the appropriate order.
  P2_type *parameter_types;

  // Not used.
  char **parameter_names;
  
  // An array of length {parameters} containing at each parameter index a zero
  // if the primitive may have side-effects on an argument passed via that,
  // or a 1 otherwise (the primitive is "referentially transparent" with respect
  // to that parameter).  Not yet used.
  char *transparency;

  P2_type return_type;
};

typedef struct P2_primitive_ P2_primitive;



P2_error P2_primitive_init();

P2_error P2_primitive_end();



void P2_register_primitive(P2_primitive *prim);

P2_primitive *P2_lookup_primitive(char *name);



#endif

/*- end of file */
