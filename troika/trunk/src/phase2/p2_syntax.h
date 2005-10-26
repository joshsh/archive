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

#ifndef P2_SYNTAX_H
#define P2_SYNTAX_H



#include "p2_atom.h"
#include "p2_error.h"
#include "p2_id.h"
#include "p2_type.h"



p2_type
   p2_int_type,
   p2_char_type,
   p2_double_type,
   p2_cstring_type;

#define P2_INT_NAME      "xs_int"
#define P2_CHAR_NAME     "xs_byte"
#define P2_DOUBLE_NAME   "xs_double"
#define P2_CSTRING_NAME  "xs_string"



p2_error p2_syntax_init();
p2_error p2_syntax_end();

// Returns an atom of type p2_error, p2_id, or any one of the basic
// data types.
p2_atom *p2_parse_token(char *s);

// Returns a p2_id atom or a p2_error atom.
p2_atom *p2_parse_id(char *s);

int p2_valid_command_name(char *s);

// Miscellaneous
//void p2_print_token(struct p2_token t);



#endif  // P2_SYNTAX_H

