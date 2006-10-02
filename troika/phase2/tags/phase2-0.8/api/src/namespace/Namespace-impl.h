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

#ifndef NAMESPACE_IMPL_H
#define NAMESPACE_IMPL_H

/*

define x
    without the final name fragment, x resolves to a namespace
    immediate parent of x is not read-only

undefine x
    x resolves
    immediate parent of x is not read-only

_rm x
    x resolves
    immediate parent of x is not read-only

_cp x y
    x resolves
    y resolves
        y is a namespace
        y is not read-only

_mv x y
    x resolves
    y resolves
        y is a namespace
        y is not read-only

*/

#include <phase2/Namespace.h>
#include "settings.h"


/** A namespace is a hash table with associated functions for adding, removing,
    and looking up named objects. */
struct Namespace
{
    /** A dictionary of objects in the namespace. */
    Dictionary *children;
};


#endif  /* NAMESPACE_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
