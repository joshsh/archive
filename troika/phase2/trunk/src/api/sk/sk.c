/*//////////////////////////////////////////////////////////////////////////////

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

*///////////////////////////////////////////////////////////////////////////////

#include "sk.h"  // p2_term
#include "../p2_primitive.h"

#include <stdlib.h>  // malloc
#include <string.h>  // memcpy



// Pointer to a function which generates output as terms are reduced.
void (*debug_print_)(p2_term *);

p2_term *S_reduce(p2_term *term);
p2_term *K_reduce(p2_term *term);
p2_term *prim_reduce(p2_term *term);

p2_term *error_term(p2_error err);



p2_error SK_init(void (*debug_print)(p2_term *))
{
    debug_print_ = debug_print;

    SK_S_type = p2_register_type("S", 0, 0, 0, 0);
    SK_K_type = p2_register_type("K", 0, 0, 0, 0);

    SK_S = p2_atom__new((void *) SK_S_type, 0);
    SK_K = p2_atom__new((void *) SK_K_type, 0);

    // Register special errors raised by SK_reduce.
    FAILURE = p2_register_error("FAILURE");
    TYPE_MISMATCH = p2_register_error("TYPE_MISMATCH");
    ATOM_APPLIED_AS_FUNCTION = p2_register_error("ATOM_APPLIED_AS_FUNCTION");
    PRIMITIVE_APPLIED_TO_NONATOM = p2_register_error("PRIMITIVE_APPLIED_TO_NONATOM");
    NULL_TERM = p2_register_error("NULL_TERM");
    NULL_ATOM = p2_register_error("NULL_ATOM");
    NULL_PRIMITIVE = p2_register_error("NULL_PRIMITIVE");
    EXPIRED_TERM = p2_register_error("EXPIRED_TERM");
    PROGRAM_ERROR = p2_register_error("PROGRAM_ERROR");

    return P2_SUCCESS;
}



p2_term *SK_reduce(p2_term *term)
{
    p2_atom *function;  //? Better descriptor?
    p2_term *result = NULL;
    p2_type type;

    // Iterate while !result.
    do
    {
        //debug_print_(term);

        // Get the atom at the head of the term.
        // Caution: the term MUST be in normal form.
        function = (p2_atom *) *(term->head + 2);

        // Attempt to apply the leftmost atom to the arguments on its right.
        type = (p2_type) function->type;

        // Apply primitive.
        if (type == p2_primitive_type)
            result = prim_reduce(term);

        // Sxyz... --> xz(yz)...
        else if (type == SK_S_type)
            S_reduce(term);

        // Kxy... --> x...
        else if (type == SK_K_type)
            result = SK_reduce(K_reduce(term));

        // Any atom which is not an S or K combinator or a p2_primitive is
        // is considered a non-redex atom.
        else
        {
            #ifdef P2FLAGS__PERMIT_IRREDUCIBLE_TERMS
                result = term;
            #else
                // Garbage-collect whatever is left of {term}
                p2_term__delete(term);

                // Generate an error.
                result = error_term(ATOM_APPLIED_AS_FUNCTION);
            #endif
        }

    } while (!result);

    return result;
}



// Kxy --> x
// [term size] [2]{K} [x_size]{x} [y_size]{y} ...
//     --> [term size - y_size - 2] [x_size]{x} ...
p2_term *K_reduce(p2_term *term)
{
    void **x, **y;
    unsigned int x_size, y_size;

    if (p2_term__length(term) < 3)
        return term;

    // Skip the 'K' to reach the head of the 'x' sub-term.
    x = term->head + 3;
    x_size = (unsigned int) *x;

    // Skip the 'x' and to reach the head of the 'y' sub-term.
    y = x + x_size;
    y_size = (unsigned int) *y;

    // Copy the 'x' subterm to the target position.
    term->head = y + (y_size - x_size);
    memcpy(term->head, x, x_size * sizeof(void *));

    // Reset the term head.
    term->head--;
    *(term->head) = (void *) (term->buffer + term->buffer_size - term->head);

    return SK_reduce(term);
}



// Sxyz --> xz(yz)
// [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
//     --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ...
p2_term *S_reduce(p2_term *term)
{
    void **x, **y, **z;
    unsigned int x_size, y_size, z_size, newsize;

    if (p2_term__length(term) < 4)
        return term;

    // Locate the head of 'x', 'y' and 'z'.
    x = term->head + 3;
    x_size = (unsigned int) *x;
    y = x + x_size;
    y_size = (unsigned int) *y;
    z = y + y_size;
    z_size = (unsigned int) *z;

    // Predict the size of the resulting array.  If necessary, copy the term to a
    // larger buffer and reduce that instead.
    newsize = (unsigned int) *(term->head) + z_size - 1;
    if (newsize > term->buffer_size)
    {
        term = p2_term__expand(term, newsize);

        // Re-locate the head of 'x', 'y' and 'z'.
        x = term->head + 3;
        x_size = (unsigned int) *x;
        y = x + x_size;
        y_size = (unsigned int) *y;
        z = y + y_size;
        z_size = (unsigned int) *z;
    }

    // Copy 'x' to an auxiliary array.
    void **aux = (void **) malloc(x_size * sizeof(void *));
    memcpy(aux, x, x_size * sizeof(void *));

    // Prepend a term head for the new sub-term 'yz'.  The data of 'y' and 'z'
    // remains where it is.
    term->head = y - 1;
    *(term->head) = (void *) (y_size + z_size + 1);

    // Prepend a duplicate 'z'.
    term->head -= z_size;
    memcpy(term->head, z, z_size * sizeof(void *));

    // Prepend an 'x'.
    term->head -= x_size;
    memcpy(term->head, aux, x_size * sizeof(void *));
    free(aux);

    // Prepend a term head.
    term->head--;
    *(term->head) = (void *) (term->buffer + term->buffer_size - term->head);

    return SK_reduce(term);
}



// Note: prim_reduce allows for at most five arguments.
p2_atom *args[10];

// Assumes head-normal form.
// Note: it's probably worth trying to find a way to consolidate the type
// checking and garbage collection of arguments.
p2_term *prim_reduce(p2_term *term)
{
    int i;
    p2_atom *arg, *return_value;
    p2_term *result;
    p2_error err = (p2_error) 0;
    void **cur = term->head + 2;
    p2_primitive *prim = ((p2_primitive *) *cur)->value;
    cur++;

    //  Insufficient number of arguments for reduction.
    if (p2_term_length(term) <= prim->parameters)
        result = term;

    else
    {
        #ifdef P2FLAGS__DO_TYPE_CHECKING
            // Type-check the arguments, bail if there is a mismatch.
            for (i = 0; i < prim->parameters; i++)
            {
                if ((unsigned int) *cur != 2)
                {
                    err = PRIMITIVE_APPLIED_TO_NONATOM;
                    break;
                }
                cur++;
                arg = (p2_atom *) *cur;
                if (arg->type != prim->parameter_types[i])
                {
                    err = TYPE_MISMATCH;
                    break;
                }
                args[i] = arg;
                cur++;
            }

        if (!err)
        #endif  // P2FLAGS__DO_TYPE_CHECKING

        // Apply the primitive.
        {
            // I'm sure this is the stupid way to type-cast a function of a
            // variable number of parameters... but it's handy enough for now.
            switch (prim->parameters)
            {
                // Note: no 0 case.
                case 1:
                    return_value = (p2_atom *) ((void *(*)(void *)) prim->value)(
                        args[0]);
                    break;
                case 2:
                    return_value = (p2_atom *) ((void *(*)(void *, void *)) prim->value)(
                        args[0], args[1]);
                    break;
                case 3:
                    return_value = (p2_atom *) ((void *(*)(void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2]);
                    break;
                case 4:
                    return_value = (p2_atom *) ((void *(*)(void *, void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2], args[3]);
                    break;
                case 5:
                    return_value = (p2_atom *) ((void *(*)(void *, void *, void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2], args[3], args[4]);
                    break;
                default:
                    err = PROGRAM_ERROR;
            }
        }

        if (err)
        {
            result = error_term(err);
            term__delete(term);
        }

        else
        {
            // Replace the primitive reference and its arguments with the return value.
            cur--;
            *cur = return_value;
            cur--;
            *cur = (void *) 2;
            term->head = cur - 1;
            *(term->head) = (void *) (term->buffer + term->buffer_size - term->head);

            // Unless the application of a primitive can yield another
            // primitive (or S or K combinator), the resulting term
            // cannot be further reduced.
            #ifdef P2FLAGS__HIGHER_ORDER_PRIMITIVES
                result = SK_reduce(term);
            #else
                result = term;
            #endif
        }
    }

    return result;
}



p2_term *error_term(p2_error err)
{
    return p2_term__new(p2_error_atom(err), 0);
}


