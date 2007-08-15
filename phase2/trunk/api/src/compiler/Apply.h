/**

\file  Apply.h

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

#ifndef APPLY_H
#define APPLY_H

#include <phase2/Object.h>

typedef struct Apply Apply;

struct Apply
{
    Object *function;
    Object *operand;
};

extern Apply *
apply__new( Object *function, Object *operand );

extern void
apply__free( Apply *a );

extern void
apply__walk( Apply *a, Visitor f );

#include <phase2/Term.h>

extern Term *
apply__as_term( Apply *a, Type *apply_type, Type *indirection_type );

#include <phase2/Manager.h>

extern Object *
term__to_apply_tree( Term *t, Manager *m, Type *apply_type );

extern void
apply__encode( Apply *a, char *buffer );

extern Type *
apply__create_type( const char *name, int flags );


#endif  /* APPLY_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
