/**
    \file flags.h

    \author Joshua Shinavier   \n
            parcour@gmail.com  \n
            +1 509 570-6990    \n */

/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#ifndef P2_FLAGS_H
#define P2_FLAGS_H


// Association /////////////////////////////////////////////////////////////////

#define P2DEF_ASSOCIATION

#ifdef P2DEF_ASSOCIATION
    #define P2DEF_OUTBOUND_EDGES
    // #define P2DEF_TRANS_EDGES
    // #define P2DEF_INBOUND_EDGES
#endif


// Memory management ///////////////////////////////////////////////////////////

#define P2DEF_MARK_AND_SWEEP

#define P2DEF_MANAGE_PRIMITIVES


// Debugging ///////////////////////////////////////////////////////////////////

/** While the gSOAP DOM parser is still struggling with namespaces... */
#define OMIT_XMLNS

//#define DEBUG
//#define PARSER_DEBUG

/** If this is commented out, the parser will pass arrays of strings, rather than
    XML, for parser arguments. */
//#define PASS_COMMAND_ARGS_AS_XML

// ! This is not a flag.
#ifdef DEBUG
    #include <stdio.h>  // printf
#endif

// ! This is not a flag.
#ifdef PARSER_DEBUG
    #define DEBUG_PRINTF printf
#else
    #define DEBUG_PRINTF
#endif


////////////////////////////////////////////////////////////////////////////////

#endif  // P2_FLAGS_H

