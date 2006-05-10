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

*****************************************************************************/

#include <Void.h>
#include "settings.h"


/** Start off with a small, out-of-range address and work upwards. */
static Void void__maxval = ( Void ) 42;


Void *
void__new()
{
    Void *v = new( Void );
    *v = ++void__maxval;
    return v;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
