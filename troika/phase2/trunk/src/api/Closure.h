/**

\file  Closure.h

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

#ifndef CLOSURE_H
#define CLOSURE_H


typedef enum _p2_action__type
{
    p2_action__type__noop = 0,

    p2_action__type__break,
    p2_action__type__remove,
    p2_action__type__replace

} p2_action__type;


typedef struct _p2_action
{
    p2_action__type type;

    /* For p2_action__type__replace: the replacement value. */
    void *value;

} p2_action;


typedef p2_action *( *procedure )( void *data, void *state );


typedef struct Closure Closure;

struct Closure
{
    procedure execute;

    /* A mutable data field which is provided to the procedure as an argument,
       and preserved between invocations. */
    void *state;
};


#define closure__execute( p, data )  (p)->execute( (data), (p)->state )


#endif  /* CLOSURE_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
