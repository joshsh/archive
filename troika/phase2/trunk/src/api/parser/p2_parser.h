/**

\file  p2_parser.h

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

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

#ifndef P2_PARSER_H
#define P2_PARSER_H

#include "../p2.h"

typedef enum _p2_parser__exit_state
{
    exit_state__aborted,
    exit_state__end_of_input,
    exit_state__locked_out,
    exit_state__parse_failure

} p2_parser__exit_state;


#endif  /* P2_PARSER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
