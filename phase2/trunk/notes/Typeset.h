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

#ifndef TYPESET_H
#define TYPESET_H


#include <phase2/Type.h>


#define TYPEVAL0(x) t_##x
#define TYPEVAL(x)  TYPEVAL0(x)


typedef struct Typeset Typeset;

struct Typeset
{
    Type
        *TYPEVAL( APPLY ),
        *TYPEVAL( ARRAY ),
        *TYPEVAL( CHARACTER ),
        *TYPEVAL( COMBINATOR ),
        *TYPEVAL( DOUBLE ),
        *TYPEVAL( INDIRECTION ),
        *TYPEVAL( INTEGER ),
        *TYPEVAL( NAME ),
        *TYPEVAL( NAMESPACE ),
        *TYPEVAL( PRIMITIVE ),
        *TYPEVAL( SET ),
        *TYPEVAL( STRING ),
        *TYPEVAL( TERM ),
        *TYPEVAL( TYPE );
};








environment__create_object(


environment__type( e, APPLY );

environment__object(

TYPEVAL( e, APPLY );

TYPEOBJ( e, APPLY );









#endif  /* TYPESET_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
