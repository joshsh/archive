/*+

  p2_command.c

  This library defines a mechanism for storing and retrieving user commands,
  where a command is a global function of the form:

      P2_error (*command_name)(int nargs, char **args) { ... }

  A command accepts zero or more arguments in the form of C strings, and returns
  an error code describing execution outcome.

  A collection of accessible commands is built up by binding each command
  function with an id via P2_register_command.  Commands are then called by
  specifying the id of the command, plus its arguments.

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

// May be separately compiled.

#include "p2_command.h"
#include "p2_syntax.h"

#include "util/array.h"
#include "util/hash_table.h"

#include <string.h>  // For strdup



// Global variables to hold all user commands.
P2_hash_table *commands_;
P2_array *command_names_;



P2_error P2_command_init()
{
    commands_ = hash_table__new(40, 2.0, 2.0, STRING_DEFAULTS);
    command_names_ = array__new(20, 1.5);

    return P2_SUCCESS;
}



P2_error P2_command_end()
{
    array__forall(command_names_, free);
    array__delete(command_names_);
    hash_table__delete(commands_);

    return P2_SUCCESS;
}



P2_error P2_register_command(char *name, COMMAND_REFERENCE(command))
{
    char *stable_name;
    P2_error err;

    // Valid command name?
    if (!P2_valid_command_name(name))
        err = INVALID_COMMAND_NAME;

    // Command name is unique?
    else if (hash_table__lookup(commands_, (void *) name) != NULL)
        err = DUPLICATE_COMMAND_NAME;

    else
    {
        stable_name = strdup(name);
        hash_table__add(commands_, (void *) stable_name, (void *) command);
        array__enqueue(command_names_, (void *) stable_name);
        err = P2_SUCCESS;
    }

    return err;
}



P2_error P2_execute_command(char *name, int nargs, char **args)
{
    P2_error err;

    void *p = hash_table__lookup(commands_, (void *) name);

    if (p == NULL)
        err = UNKNOWN_COMMAND;

    else
        err = ((COMMAND_FORMAT) p)(nargs, args);

    return err;
}



/*- end of file */
