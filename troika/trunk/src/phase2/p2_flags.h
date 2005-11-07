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

#define P2FLAGS__ASSOCIATION

#ifdef P2FLAGS__ASSOCIATION
    // #define P2FLAGS__TRANS_EDGES
    // #define P2FLAGS__INBOUND_EDGES

    #define P2FLAGS__ATOM__INIT_ASSOC_BUFFER_SIZE  0
#endif


// Memory management ///////////////////////////////////////////////////////////

#define P2FLAGS__MARK_AND_SWEEP

#ifdef P2FLAGS__MARK_AND_SWEEP

    #define P2FLAGS__MARKANDSWEEP_QUEUE__BLOCK_SIZE  1000
    #define P2FLAGS__MARKANDSWEEP_ATOMS__BLOCK_SIZE  1000
    #define P2FLAGS__INIT_MARKANDSWEEP_BUFFER_SIZE   1000

    #ifdef P2FLAGS__ASSOCIATION
        #define P2FLAGS__INIT_TRIGGER_BUFFER_SIZE  10
    #endif

#endif


#define P2FLAGS__MANAGE_PRIMITIVES


// Debugging ///////////////////////////////////////////////////////////////////

/** While the gSOAP DOM parser is still struggling with namespaces... */
#define P2FLAGS__OMIT_XMLNS

//#define P2FLAGS__DEBUG

// ! This is not a flag.
#ifdef P2FLAGS__DEBUG
    #include <stdio.h>  // printf
#endif


////////////////////////////////////////////////////////////////////////////////

#endif  // P2_FLAGS_H

