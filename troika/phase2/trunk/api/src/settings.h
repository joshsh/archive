/**
    \file  settings.h

    \author  Joshua Shinavier   \n
             parcour@gmail.com  \n
             +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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


/* Define required macros (if not already defined via autotools) **************/


#ifndef PACKAGE_NAME
#   define PACKAGE_NAME                         "phase2"
#endif

#ifndef PACKAGE_VERSION
#   define PACKAGE_VERSION                      "?"
#endif

#ifndef PACKAGE_BUGREPORT
#   define PACKAGE_BUGREPORT                    "parcour@gmail.com"
#endif

#ifndef HAVE_LIBNCURSES
#   define HAVE_LIBNCURSES                      0
#endif

#ifndef HAVE_LIBREADLINE
#   define HAVE_LIBREADLINE                     0
#endif


/******************************************************************************/


#include <stdlib.h>  /* malloc */
#include <string.h>  /* strlen, strcpy */

#define STRDUP(x)   strcpy(malloc(1 + strlen(x)), (x))
#define MALLOC      malloc
#define NEW(type)   malloc(sizeof (type))


/******************************************************************************/


#define NOPROPS                                 0
#define NOFLAGS                                 0


#ifndef TRUE
#   define TRUE                                 1
#endif

#ifndef FALSE
#   define FALSE                                0
#endif


/* Type names *****************************************************************/


#define NAMEOF_0(x)                             #x
#define NAMEOF(x)                               NAMEOF_0(x)

#define ANY                                     any_type
#define APPLY                                   Apply
#define BAG                                     Bag
#define CHARACTER                               char
#define COMBINATOR                              Combinator
#define DOUBLE                                  double
#define INDIRECTION                             Indirection
#define INTEGER                                 int
#define NAMESPACE                               Namespace
#define PRIMITIVE                               Primitive
#define SET                                     Set
#define STRING                                  cstring
#define TERM                                    Term
#define TYPE                                    Type
#define VOID                                    Void

#define DATA_SET                                data_set
#define OBJECT                                  object
#define TRIPLE                                  triple

/*
#define ENCODING__ROOT__XML__NAME               "data_set"

#define OBJECT__XML__NAME                       "object"
#define TRIPLES__XML__NAME                      "triple"

#define APPLY__XML__NAME                        "apply"
#define ARRAY__XML__NAME                        "list"
#define NAMESPACE__XML__NAME                    "table"
#define SET__XML__NAME                          "list"
#define TERM__XML__NAME                         "list"
*/


/* Reduction ******************************************************************/


struct Object;

extern struct Object *
dereference( struct Object ** );

#define DEREF   dereference


/** Tolerate null object references in various contexts. */
#define FIRST_CLASS_NULL                        1

/** Short out indirection nodes in reduction algorithms as well as in
    memory management algorithms.  May be a costly feature. */
#define EAGER_REDIRECTION                       1

/** Replace any useless apply nodes (i.e. those which are equivalent to NULL)
    with indirection nodes to NULL whenever they are encountered.  This may
    repetitious traversals of the same null-terminated spine.  The disadvantage
    is time wasted on expressions which will never be used again, anyway. */
#define EAGER_PRUNING                           1

#define PERMIT_TEMPORARY_OBJECTS                1


#define PRIM__IMPLICIT_SET_MAPPING              1
#define PRIM__ALLOW_GENERIC_PARAMS              1
#define PRIM__ALLOW_GENERIC_RESULTS             1

/** An arbitary limit on the number of parameters to a built-in function. */
#define PRIM__MAX_PARAMS                        16

/** Allow the possibility of a primitive (for instance, the ^ operator)
    yielding another primitive as a return value. */
#define PRIM__ALLOW_HIGHER_ORDER                1

#define PRIM__ALLOW_NULLARY                     0
#define PRIM__ALLOW_VOID                        0

#define PRIM__CHECKS__PARAM_TYPE                1


/** Whether to use graph vs. tree reduction algorithms. */
#define SK__REDUCE_AS_GRAPH                     1

#define SK__CHECKS__APPLY_TO_NONATOM            1

#define SK__CHECKS__MAX_TERM_SIZE               100000
#define SK__CHECKS__MAX_REDUX_ITERATIONS        10000


/** Allow non-redex atoms at the beginning of an expression, (or as arguments to
    a function) and simply abandon reduction when they are encountered. */
#define SK__ALLOW_NONREDUX                      0

#define SK__IMPLICIT_ASSOCIATION                1


/* Compiler interface *********************************************************/


#define COMPILER__NAME_INHERITANCE              1


/* Serialization **************************************************************/

#define SERIAL__CHECKS                          1

#define ENCODING__BUFFER_SIZE                   0x10000

#define ENCODING__TRIPLES_AS_OBJECTS            1

#define APPLY__XML__NAME                        "apply"
#define ARRAY__XML__NAME                        "list"
#define NAMESPACE__XML__NAME                    "table"
#define SET__XML__NAME                          "list"
#define TERM__XML__NAME                         "list"


/* Triples / association ******************************************************/


#define TRIPLES                                 1
#if TRIPLES
#   define TRIPLES__GLOBAL                      1
#   if TRIPLES__GLOBAL
#       define TRIPLES__GLOBAL__IN_EDGES        0
#       define TRIPLES__GLOBAL__OUT_EDGES       1
#       define TRIPLES__GLOBAL__TRANS_EDGES     0
#   endif
#    /** Subject ==> Predicate */
#   define TRIPLES__IMPLICATION__S_P            1
#   if !TRIPLES__IMPLICATION__S_P
#       /** Subject + Object ==> Predicate */
#       define TRIPLES__IMPLICATION__SO_P       0
#   endif
#   /** Subject ==> Object */
#   define TRIPLES__IMPLICATION__S_O            1
#   /** Subject + Predicate ==> Object */
#   if !TRIPLES__IMPLICATION__S_O
#       define TRIPLES__IMPLICATION__SP_O       0
#   endif
#endif  /* TRIPLES__GLOBAL */


/* Memory management **********************************************************/


#define MEM__MARK_AND_SWEEP                     1

#if MEM__MARK_AND_SWEEP
#  define MEM__OBJECTS__BLOCK_SIZE              1000
#  define MEM__COLLECTION_THRESHOLD             1.5
#endif  /* P2FLAGS__MARK_AND_SWEEP */


/* Terminal *******************************************************************/

/* TODO: USE_NCURSES is not yet supported */
#define USE_NCURSES                             0

#if USE_NCURSES
#    include <ncurses.h>
#    define PRINT                               printw
#    define REFRESH                             refresh
#else
#    define PRINT                               printf
#    define REFRESH
#endif


/* Errors *********************************************************************/


#include <stdio.h>  /* fprintf */

#define FFLUSH  { fflush( stdout ); fflush( stderr ); }

/* Note: the GCC extension is used here instead of __VA_ARGS__ for compatibility
   with old GCC cross-compilers. */

#define ERROR(args...) (                                                    \
    fprintf( stderr, "ERROR: " ),                                           \
    fprintf( stderr , ##args ),                                             \
    fprintf( stderr, "\n" ),                                                \
    fflush( stderr ) )

#define WARNING(args...) (                                                  \
    fprintf( stderr, "WARNING: " ),                                         \
    fprintf( stderr , ##args ),                                             \
    fprintf( stderr, "\n" ),                                                \
    fflush( stderr ) )


/* Debugging ******************************************************************/


#define DEBUG__SAFE                             1

/* TODO: these are barely used */
#define DEBUG                                   0
#    define DEBUG__MAIN                         1 & DEBUG
#    define DEBUG__CORE                         1 & DEBUG
#        define DEBUG__SK                       1 & DEBUG__CORE
#        define DEBUG__PRIMS                    1 & DEBUG__CORE
#        define DEBUG__TYPE                     1 & DEBUG__CORE
#        define DEBUG__MEMORY                   1 & DEBUG__CORE
#        define DEBUG__OBJECT                   1 & DEBUG__CORE
#            define DEBUG__OBJECT__TRACE        0 & DEBUG__OBJECT
#            define DEBUG__OBJECT__TRIPLES      1 & DEBUG__OBJECT
#        define DEBUG__NAMESPACE                0 & DEBUG__CORE
#        define DEBUG__ENV                      0 & DEBUG__CORE
#        define DEBUG__COMPILER                 1 & DEBUG__CORE
#        define DEBUG__SERIAL                   0 & DEBUG__CORE
#    define DEBUG__PARSER_MODULE                1 & DEBUG
#        define DEBUG__AST                      1 & DEBUG__PARSER_MODULE
#        /** Echo each token as it is matched by the lexer. */
#        define DEBUG__LEXER                    1 & DEBUG__PARSER_MODULE
#        /** Echo each production as it is matched by the parser. */
#        define DEBUG__PARSER                   1 & DEBUG__PARSER_MODULE
#            /** Use Bison's built-in trace facility. */
#            define DEBUG__PARSER__USE_YYDEBUG  0 & DEBUG__PARSER
#    define DEBUG__COLL                         1 & DEBUG
#        define DEBUG__ARRAY                    1 & DEBUG__COLL
#        define DEBUG__BAG                      1 & DEBUG__COLL
#        define DEBUG__DICTIONARY               1 & DEBUG__COLL
#        define DEBUG__BUNCH                    1 & DEBUG__COLL
#        define DEBUG__HASH_TABLE               1 & DEBUG__COLL
#        define DEBUG__LOOKUP_TABLE             1 & DEBUG__COLL
#        define DEBUG__NAME                     1 & DEBUG__COLL
#        define DEBUG__SET                      1 & DEBUG__COLL
#        define DEBUG__TERM                     1 & DEBUG__COLL

#define COMPILER__SHOW_ADDRESS                  DEBUG


#endif  /* SETTINGS_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
