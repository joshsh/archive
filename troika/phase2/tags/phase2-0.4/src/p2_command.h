/*+

  p2_command.h

  This library defines a mechanism for storing and retrieving user commands,
  where a command is a global function of the form:

      enum P2_error (*command_name)(int nargs, char **args) { ... }

  A command accepts zero or more arguments in the form of C strings, and returns
  an error code describing execution outcome.

  A collection of accessible commands is built up by binding each command
  function with an id via P2_register_command.  Commands are then called by
  specifying the id of the command, plus its arguments.

  last edited: 5/16/05

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

#ifndef P2_COMMAND_H
#define P2_COMMAND_H



#include "p2.h"



#define COMMAND_FORMAT			P2_error (*)(int, char **)
#define COMMAND_REFERENCE(param)	P2_error (* param )(int, char **)
#define COMMAND(function_name)		P2_error function_name (int nargs, char **args)



P2_error P2_command_init();
P2_error P2_command_end();



P2_error P2_register_command(char *name, COMMAND_REFERENCE(command));



// Pass the name of the command to execute and an array of arguments (nargs is
// the dimension of the array).
P2_error P2_execute_command(char *name, int nargs, char **args);



#endif

/*- end of file */
