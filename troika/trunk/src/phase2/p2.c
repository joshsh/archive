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
#include "util/term.h"
#include <stdlib.h>  // For NULL.



#ifdef MARK_AND_SWEEP
P2_term *error_atoms_;
#endif



P2_error P2_init()
{
    P2_error err;
    
    P2_term__init(2.0);
    
    // Note: P2_type_init must be called before P2_error_init.
    if (!(err = P2_type_init())
        &&!(err = P2_error_init())
        &&!(err = P2_primitive_init())
	&&!(err = P2_atom_init())
        ) err = P2_SUCCESS;

    // Register P2_identifier type.
    P2_id_type = P2_register_type(P2_ID_NAME,
        (ENCODE_FORMAT) NULL,
        (DECODE_FORMAT) NULL,
        (DESTROY_FORMAT) P2_id__delete,
        (CLONE_FORMAT) NULL);	

    // Create a term containing each P2_error in registered_errors.
    /*
    size = registered_errors_->size;
    P2_atom *atom = P2_atom_new(P2_error_type, (void *) err);
    P2_term *term = P2_term__new(atom);
    if (!error_atoms_)
        error_atoms_ = term;
    else
        error_atoms_ = P2_term__cat(error_atoms_, term);
	*/
    
    return err;
}



P2_error P2_end()
{
    P2_error err = P2_SUCCESS, err2;

    P2_term__end();

    // (g++ suggested the extra parens).
    // Garbage collection must be performed *before* the type interfaces are
    // destroyed.
    if ((err2 = P2_atom_end()))
        err = err2;
    if ((err2 = P2_error_end()))
        err = err2;
    if ((err2 = P2_primitive_end()))
        err = err2;
    if ((err2 = P2_type_end()))
        err = err2;
    
    return err;
}



#ifdef MARK_AND_SWEEP
P2_atom *P2_error_atom(P2_error err)
{
    return (P2_atom *)
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
    P2_init();
    P2_end();
}
//*/



/*- end of file */
