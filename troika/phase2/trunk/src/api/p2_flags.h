/**
    \file  p2_flags.h

    \brief  Global includes and macros.

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

#ifndef P2_FLAGS_H
#define P2_FLAGS_H


/* Errors / debugging *********************************************************/

#include <stdio.h>  /* fprintf */

#define PRINTERR( msg )  fprintf( stderr, "Error: %s.\n", msg )

#include <stdlib.h>  /* malloc */

#define new( type )  ( type* ) malloc( sizeof( type ) )

#define DEBUG__SAFE 1


/* Reduction ******************************************************************/

#define P2FLAGS__DO_TYPE_CHECKING

#define P2FLAGS__HIGHER_ORDER_PRIMITIVES  0

/** If defined, allow non-redex atoms at the beginning of an expression, and
   simply abandon reduction when they are encountered (rather than raising an
   error). */
#define P2FLAGS__PERMIT_IRREDUCIBLE_TERMS


/* Triples / association ******************************************************/

#define TRIPLES 1

#if TRIPLES

    #define TRIPLES__GLOBAL    1

    #if TRIPLES__GLOBAL

        #define TRIPLES__GLOBAL__OBJECT_INIT_BUFFER_SIZE   0

        #define TRIPLES__GLOBAL__IN_EDGES   0
        #define TRIPLES__GLOBAL__OUT_EDGES  1
        #define TRIPLES__GLOBAL__TRANS_EDGES    0

    #endif

    /** Subject ==> Predicate */
    #define TRIPLES__IMPLICATION__S_P   1

    #if !TRIPLES__IMPLICATION__S_P
        /** Subject + Object ==> Predicate */
        #define TRIPLES__IMPLICATION__SO_P  0
    #endif

    /** Subject ==> Object */
    #define TRIPLES__IMPLICATION__S_O   1

    /** Subject + Predicate ==> Object */
    #if !TRIPLES__IMPLICATION__S_O
        #define TRIPLES__IMPLICATION__SP_O  0
    #endif

#endif  /* TRIPLES__GLOBAL */


/* Memory management **********************************************************/

/* For p2_object's flags field. */
#define OBJECT__IS_OBJ_COLL ( int ) 0x001
#define OBJECT__MARKED      ( int ) 0x010
#define OBJECT__OWNED       ( int ) 0x100

#define MEM__MARK_AND_SWEEP 1

#if MEM__MARK_AND_SWEEP

    #define P2FLAGS__MARKANDSWEEP_QUEUE__BLOCK_SIZE 1000
    #define MEM_MANAGER__OBJECTS__BLOCK_SIZE        1000
    #define P2FLAGS__INIT_MARKANDSWEEP_BUFFER_SIZE  1000

    #if TRIPLES__GLOBAL
        #define P2FLAGS__INIT_TRIGGER_BUFFER_SIZE  10
    #endif

#endif  /* P2FLAGS__MARK_AND_SWEEP */


#define P2FLAGS__MANAGE_PRIMITIVES  1


/******************************************************************************/

#endif  /* P2_FLAGS_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
