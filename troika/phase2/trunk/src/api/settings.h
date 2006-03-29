/**
    \file  settings.h

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

#ifndef SETTINGS_H
#define SETTINGS_H


#define GNU_EXTENSIONS                          1


/* Debugging ******************************************************************/


#define DEBUG__SAFE                             1

#define DEBUG                                   1
#if DEBUG

    #define DEBUG__CORE                         1
    #if DEBUG__CORE

        #define DEBUG__SK                       1
        #define DEBUG__PRIMS                    1

        #define DEBUG__TYPE                     1
        #define DEBUG__OBJECT                   1
        #define DEBUG__MEMORY                   1

        #define DEBUG__NAMESPACE                1

        #define DEBUG__ENV                      1
        #define DEBUG__COMPILER                 1

    #endif

    #define DEBUG__PARSER_MODULE                0
    #if DEBUG__PARSER_MODULE

        #define DEBUG__AST                      1

        /** Echo each token as it is matched by the lexer. */
        #define DEBUG__LEXER                    1

        /** Echo each production as it is matched by the parser. */
        #define DEBUG__PARSER                   1
        #if DEBUG__PARSER

            /** Use Bison's built-in trace facility. */
            #define DEBUG__PARSER__USE_YYDEBUG  0

        #endif

    #endif

    #define DEBUG__UTILS                        0
    #if DEBUG__UTILS

        #define DEBUG__ARRAY                    1
        #define DEBUG__BAG                      1
        #define DEBUG__BUNCH                    1
        #define DEBUG__HASH_TABLE               1
        #define DEBUG__LOOKUP_TABLE             1
        #define DEBUG__NAME                     1
        #define DEBUG__SET                      1
        #define DEBUG__TERM                     1

    #endif

#endif


/* Reduction ******************************************************************/


#define PRIM__ALLOW_HIGHER_ORDER                0
#define PRIM__ALLOW_NOARG_FUNCTIONS             0
#define PRIM__ALLOW_VOID_FUNCTIONS              0

#define PRIM__CHECKS__PARAM_TYPE                1


#define SK__CHECKS__APPLY_TO_NONATOM            1

#define SK__CHECKS__MAX_TERM_SIZE               1000
#define SK__CHECKS__MAX_REDUX_ITERATIONS        1000


/** Allow non-redex atoms at the beginning of an expression, and simply abandon
   reduction when they are encountered. */
#define SK__ALLOW_NONREDUX                      1

#define SK__IMPLICIT_ASSOCIATION                1


/* Serialization **************************************************************/

#define ENCODING__TRIPLES_AS_OBJECTS            1

#define ENCODING__ROOT__XML__NAME               "data_set"

#define TRIPLES__XML__NAME                      "triple"
#define OBJECT__XML__NAME                       "object"

#define ARRAY__XML__NAME                        "list"
#define NAMESPACE__XML__NAME                    "table"
#define TERM__XML__NAME                         "list"


/* Triples / association ******************************************************/


#define TRIPLES 1

#if TRIPLES

    #define TRIPLES__GLOBAL                     1

    #if TRIPLES__GLOBAL

        #define TRIPLES__GLOBAL__IN_EDGES       0
        #define TRIPLES__GLOBAL__OUT_EDGES      1
        #define TRIPLES__GLOBAL__TRANS_EDGES    0

    #endif

    /** Subject ==> Predicate */
    #define TRIPLES__IMPLICATION__S_P           1

    #if !TRIPLES__IMPLICATION__S_P
        /** Subject + Object ==> Predicate */
        #define TRIPLES__IMPLICATION__SO_P      0
    #endif

    /** Subject ==> Object */
    #define TRIPLES__IMPLICATION__S_O           1

    /** Subject + Predicate ==> Object */
    #if !TRIPLES__IMPLICATION__S_O
        #define TRIPLES__IMPLICATION__SP_O      0
    #endif

#endif  /* TRIPLES__GLOBAL */


/* Memory management **********************************************************/


#define MEM__MARK_AND_SWEEP                     1

#if MEM__MARK_AND_SWEEP

    #define MEM__OBJECTS__BLOCK_SIZE            1000

#endif  /* P2FLAGS__MARK_AND_SWEEP */


#define P2FLAGS__MANAGE_PRIMITIVES              1


/******************************************************************************/


#endif  /* SETTINGS_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
