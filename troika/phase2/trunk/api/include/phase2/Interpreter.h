/**

\file  Interpreter.h

\brief  Provides the means to interact with a data set, evaluating expressions
and commands, and translating data sets into and out of specialized XML
documents.

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

#ifndef INTERPRETER_H
#define INTERPRETER_H


#include <phase2/Environment.h>
#include <phase2/collection/Dictionary.h>
#include <phase2/collection/Name.h>


/** An object which serves as a user interface to a Phase2 environment. */
typedef struct Interpreter Interpreter;


/** Constructor.
    \param env  an environment for the compiler to act upon */
extern Interpreter *
interpreter__new( Environment *env, boolean quiet );


/** Destructor. */
extern void
interpreter__delete( Interpreter *itp );


/** \return  the interpreter's associated environment */
extern Environment *
interpreter__environment( Interpreter *itp );


/** \return  the interpreter's working namespace (object names are resolved
    relative to this namespace) */
extern OBJ( NAMESPACE ) *
interpreter__working_namespace( Interpreter *itp );


/** Adds a statement or command to the interpreter's history.  This is only
    useful if readline support is enabled. */
extern void
interpreter__add_to_history( const char *s );

/** Writes interpreter history to standard out. */
extern void
interpreter__print_history();

/** Saves interpreter history to a file. */
extern void
interpreter__write_history( const char *file );


/******************************************************************************/

/** \return  the name of an object in a given namespace, if reachable from that
    namespace.  The name is qualified only to the extent necessary to resolve
    the object on a subsequent name lookup. */
extern Name *
interpreter__name_of( Interpreter *itp, OBJ( NAMESPACE ) *nso, const Object *o );

/** \return  the fully-qualified name of an object in a given namespace, if
    reachable from that namespace */
extern Name *
interpreter__name_of__full( Interpreter *itp, OBJ( NAMESPACE ) *nso, const Object *o );

extern void
interpreter__encode( Interpreter *itp,
                     const Object *o,
                     char *buffer,
                     unsigned int maxlen );

/** \return  a dot graph representation of the object */
extern char *
interpreter__draw( Interpreter *itp, const Object *o );


/******************************************************************************/


/** Serialize a data set to a file in the p2 format. */
extern void
interpreter__serialize( Interpreter *itp, const char *path );

/** Deserialize a data set from a file in the p2 format. */
extern void
interpreter__deserialize( Interpreter *itp, const char *path );


/* Externally linked functions for the parser *********************************/


/** Interprets and carries out a command (e.g. "_saveas temp.p2;").
    \return  whether to end the current session */
extern int
interpreter__evaluate_command( Interpreter *itp, OBJ( STRING ) *name, OBJ( ARRAY ) *args, const char *text );

/** Interprets and evaluates an expression in the programming environment.
    \param name  if given, the resulting object will be bound to the working
    namespace by this name
    \return  whether to end the current session */
extern int
interpreter__evaluate_expression( Interpreter *itp, OBJ( NAME ) *name, Object *expr, const char *text );

/** Prints a parse error.
    \return  whether to end the current session
    \note  Error messages are not suppressed by the --quiet option. */
extern int
interpreter__handle_parse_error( Interpreter *itp, const char *msg );

/** \return  whether the lexer and parser are to avoid printing to stdout while
    matching input */
extern boolean
interpreter__quiet( Interpreter *c );

/** \return  whether a line number is printed before each new line of input */
extern boolean
interpreter__show_line_numbers( Interpreter *itp );


#endif  /* INTERPRETER_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
