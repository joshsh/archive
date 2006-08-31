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

#ifndef INTERPRETER_IMPL_H
#define INTERPRETER_IMPL_H


#include <Interpreter.h>
#include <compiler.h>

#include "../object/Object-impl.h"

#include <time.h>


struct Interpreter
{
    Environment *env;
    OBJ( NAMESPACE ) *cur_ns_obj;

    Dictionary *commands;

    boolean quiet, show_line_numbers;

    /* Parser types. */
    Type *apply_t, *bag_t, *char_t, *float_t, *int_t, *string_t, *term_t;

    /* Other types. */
    Type *combinator_t, *ns_t, *prim_t, *set_t, *type_t;

    Type *name_t;

    char *save_to_path;

    boolean use_readline;
};


extern Object *
interpreter__define( Interpreter *c, Name *name, Object *o );

extern Object *
interpreter__undefine( Interpreter *c, Name *name );

extern Object *
interpreter__resolve( Interpreter *c, Name *name );


extern Dictionary *
create_commands( void );

extern void
delete_commands( Dictionary *commands );


extern void
err_notdefined( Name *name );

extern void
err_notns( Name *name );


#endif  /* INTERPRETER_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
