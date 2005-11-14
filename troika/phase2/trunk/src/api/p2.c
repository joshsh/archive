/*+

  p2.c

  Core library for the Phase2 programming language.
  
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

#include "p2.h"
#include "util/p2_term.h"
#include <stdlib.h>  // For NULL.



#ifdef P2FLAGS__MARK_AND_SWEEP
p2_term *error_atoms_;
#endif



p2_error p2_init()
{
    p2_error err;

    p2_p2_term__init(2.0);

    // Note: p2_type_init must be called before p2_error_init.
    if (!(err = p2_type_init())
        &&!(err = p2_error_init())
        &&!(err = p2_primitive_init())
	&&!(err = p2_atom_init())
        ) err = P2_SUCCESS;

    // Register p2_identifier type.
    p2_id_type = p2_register_type(P2_ID_NAME,
        (ENCODE_FORMAT) NULL,
        (DECODE_FORMAT) NULL,
        (DESTROY_FORMAT) p2_id__delete,
        (CLONE_FORMAT) NULL);	

    // Create a term containing each p2_error in registered_errors.
    /*
    size = registered_errors_->size;
    p2_atom *atom = p2_atom_new(p2_error_type, (void *) err);
    p2_term *term = p2_p2_term__new(atom);
    if (!error_atoms_)
        error_atoms_ = term;
    else
        error_atoms_ = p2_p2_term__cat(error_atoms_, term);
	*/

    return err;
}



p2_error p2_end()
{
    p2_error err = P2_SUCCESS, err2;

    p2_p2_term__end();

    // (g++ suggested the extra parens).
    // Garbage collection must be performed *before* the type interfaces are
    // destroyed.
    if ((err2 = p2_atom_end()))
        err = err2;
    if ((err2 = p2_error_end()))
        err = err2;
    if ((err2 = p2_primitive_end()))
        err = err2;
    if ((err2 = p2_type_end()))
        err = err2;

    return err;
}



#ifdef P2FLAGS__MARK_AND_SWEEP
p2_atom *p2_error_atom(p2_error err)
{
    return (p2_atom *)
        *(error_atoms_->head + 1 + ((int) err * 2));
}
#endif



/////////////////////////////////////////////////////////////////////////////////



/*
   Debugging main()

   gcc -c util/hash_table.c
   gcc -c util/array.c
   gcc p2.c p2_primitive.c p2_error.c p2_type.c hash_table.o array.o
*//*
int main()
{
    p2_init();
    p2_end();
}
//*/



/*- end of file */
