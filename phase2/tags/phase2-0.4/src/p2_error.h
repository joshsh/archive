/*+

  p2_error.h

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

#ifndef P2_ERROR_H
#define P2_ERROR_H

#include "p2.h"



#define P2_ERROR_NAME   "P2_error"



// Error codes for a variety of contexts (custom errors may be created with
// P2_register_error).
P2_error

	P2_SUCCESS,

	// Bad token syntax
	INVALID_TOKEN,
	INVALID_IDENTIFIER,
	INVALID_COMMAND_NAME,  // properly, a program-level error.

	// Item not found
	UNKNOWN_COMMAND,
	UNKNOWN_SYMBOL,
	UNKNOWN_VARIABLE,
	UNKNOWN_DATASET,

	// Bad command call
	BAD_ARGUMENT,
	TOO_FEW_ARGUMENTS,
	TOO_MANY_ARGUMENTS,

	REDUX_FAILURE,
	
	// Program-level errors
	DUPLICATE_COMMAND_NAME,
	BAD_REGEX,
	UNEXPECTED_VALUE;



P2_error P2_error_init();
P2_error P2_error_end();

P2_error P2_register_error(char *name);



void P2_print_error(P2_error err, char *buffer);



#endif

/*- end of file */
