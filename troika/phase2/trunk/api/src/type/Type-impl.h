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

#ifndef TYPE_IMPL_H
#define TYPE_IMPL_H


#include <Type.h>
#include "../settings.h"


struct Type
{
    /** */
    char *name;

    int flags;

    Type *type_arg;

    /** Copy constructor. */
    Copy_Cons       clone;

    Comparator      compare_to;

    /** Deserializer. */
    Decoder         decode;

    /** Destructor. */
    Destructor      destroy;

    /** Serializer. */
    Encoder         encode;
    Encoder_Safe    encode_safe;

    Criterion2      equals;

    Size_Of         size;

    Sort            sort;

    Walker          walk;
};


#endif  /* TYPE_IMPL_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
