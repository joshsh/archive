/*******************************************************************************

Wurfel programming language, Copyright (C) 2006 Joshua Shinavier.

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

#ifndef MODEL_IMPL_H
#define MODEL_IMPL_H

#include <wurfel/ModelVector.h>
#include "../settings.h"


#define MODELVECTOR__INITIAL_BUFFER_SIZE        23
#define MODELVECTOR__LOAD_FACTOR                0.75
#define MODELVECTOR__EXPANSION_FACTOR           2.0


struct ModelVector
{
    unsigned int size;
    unsigned int capacity;

    index *      buffer;
    unsigned int buffer_size;
};


#endif  /* MODEL_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
