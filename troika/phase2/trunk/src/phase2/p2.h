/*+

  p2.h

  Core library for the Phase2 programming language.
  
  Note: the prototypes for p2_error, p2_type and p2_primitive are just here for
  reference.

  last edited: 9/5/05

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

#ifndef p2_H
#define p2_H	1



#define MARK_AND_SWEEP 1
#define PERMIT_IRREDUCIBLE_TERMS 1
//#define HIGHER_ORDER_PRIMITIVES

#ifndef NULL
  #define NULL (void *) 0
#endif


/*
  [ba] don't SK-reduce null terms, nor terms which don't begin with Kxy or Sxyz.
  [ba] will SK_S, SK_K collide with other "special" addresses/identifiers?
  [ba] name collision ^ TERM, p2_term -- maybe use one or the other.  Maybe
       re-name JS_array to p2_array, etc.
*/

#include "p2_flags.h"

#include "p2_atom.h"
#include "p2_error.h"
#include "p2_primitive.h"
#include "p2_type.h"



typedef void * p2_function_ptr; //~

p2_type p2_id_type;

#include "p2_id.h"



p2_error p2_init();
p2_error p2_end();



#ifdef MARK_AND_SWEEP
p2_atom *p2_error_atom(p2_error err);
#endif



#endif

/*- end of file */
