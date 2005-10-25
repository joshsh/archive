/*+

  p2_syntax.h

  Token analyzer for the Phase2 programming environment.

  last edited: 5/31/05

  Note: id and string literal parsing has a known quirk: anything containing
  a '&' character does not appear in output.  Investigate when you get a chance.

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

#ifndef P2_PARSE_H
#define P2_PARSE_H	1



#include "p2_atom.h"
#include "p2_error.h"
#include "p2_id.h"
#include "p2_type.h"



P2_type
   P2_int_type,
   P2_char_type,
   P2_double_type,
   P2_cstring_type;

#define P2_INT_NAME      "xs_int"
#define P2_CHAR_NAME     "xs_byte"
#define P2_DOUBLE_NAME   "xs_double"
#define P2_CSTRING_NAME  "xs_string"



P2_error P2_syntax_init();
P2_error P2_syntax_end();

// Returns an atom of type P2_error, P2_id, or any one of the basic
// data types.
P2_atom *P2_parse_token(char *s);

// Returns a P2_id atom or a P2_error atom.
P2_atom *P2_parse_id(char *s);

int P2_valid_command_name(char *s);

// Miscellaneous
//void P2_print_token(struct P2_token t);



#endif

/*- end of file */
