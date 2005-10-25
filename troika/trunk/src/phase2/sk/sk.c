/*+

  sk.c

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few potential logical errors and programming mistakes.

  last edited: 9/5/05

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

 *//////////////////////////////////////////////////////////////////////////////

#include "sk.h"
#include "../p2_primitive.h"


// Pointer to a function which generates output as terms are reduced.
void (*debug_print_)(P2_term *);

P2_term *S_reduce(P2_term *term);
P2_term *K_reduce(P2_term *term);
P2_term *prim_reduce(P2_term *term);
P2_term *error_term(P2_error err);



P2_error SK_init(void (*debug_print)(P2_term *))
{
    debug_print_ = debug_print;

    SK_S_type = P2_register_type("S", NULL, NULL, NULL, NULL);
    SK_K_type = P2_register_type("K", NULL, NULL, NULL, NULL);

    SK_S = P2_atom__new((void *) SK_S_type, NULL);
    SK_K = P2_atom__new((void *) SK_K_type, NULL);

    // Register special errors raised by SK_reduce.
    FAILURE = P2_register_error("FAILURE");
    TYPE_MISMATCH = P2_register_error("TYPE_MISMATCH");
    ATOM_APPLIED_AS_FUNCTION = P2_register_error("ATOM_APPLIED_AS_FUNCTION");
    PRIMITIVE_APPLIED_TO_NONATOM = P2_register_error("PRIMITIVE_APPLIED_TO_NONATOM");
    TOO_MANY_PARAMETERS = P2_register_error("TOO_MANY_PARAMETERS");
    NULL_TERM = P2_register_error("NULL_TERM");
    NULL_ATOM = P2_register_error("NULL_ATOM");
    NULL_PRIMITIVE = P2_register_error("NULL_PRIMITIVE");
    EXPIRED_TERM = P2_register_error("EXPIRED_TERM");
    PROGRAM_ERROR = P2_register_error("PROGRAM_ERROR");

    return P2_SUCCESS;
}



P2_term *SK_reduce(P2_term *term)
{
    P2_atom *function;  //? Better descriptor?
    P2_term *result = NULL;
    P2_type type;

    // Iterate while !result.
    do
    {
        //debug_print_(term);

        // Get the atom at the head of the term.
        // Caution: the term MUST be in normal form.
        function = (P2_atom *) *(term->head + 2);

        // Attempt to apply the leftmost atom to the arguments on its right.
        type = (P2_type) function->type;

        // Apply primitive.
        if (type == P2_primitive_type)
            result = prim_reduce(term);

        // Sxyz... --> xz(yz)...
        else if (type == SK_S_type)
            S_reduce(term);

        // Kxy... --> x...
        else if (type == SK_K_type)
            result = SK_reduce(K_reduce(term));

        // Any atom which is not an S or K combinator or a P2_primitive is
        // is considered a non-redex atom.
        else
        {
            #ifdef PERMIT_IRREDUCIBLE_TERMS
                result = term;
            #else
                // Garbage-collect whatever is left of {term}
                P2_term__delete(term);

                // Generate an error.
                result = error_term(ATOM_APPLIED_AS_FUNCTION);
            #endif
        }

    } while (!result);

    return result;
}



//Kxy --> x
// [term size] [2]{K} [x_size]{x} [y_size]{y} ...
//     --> [term size - y_size - 2] [x_size]{x} ...
P2_term *K_reduce(P2_term *term)
{
    void **x, **y;
    unsigned int x_size, y_size;

    if (P2_term__length(term) < 3)
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



//Sxyz --> xz(yz)
// [term size] [2]{S} [x_size]{x} [y_size]{y} [z_size]{z} ...
//     --> [term size + z_size - 1] [x_size]{x} [z_size]{z} [y_size + z_size + 1] [y_size]{y} [z_size]{z} ...
P2_term *S_reduce(P2_term *term)
{
    void **x, **y, **z;
    unsigned int x_size, y_size, z_size, newsize;

    if (P2_term__length(term) < 4)
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
        term = P2_term__expand(term, newsize);

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
P2_atom *args[10];

// Assumes head-normal form and a sufficient number of arguments.
// Note: it's probably worth trying to find a way to consolidate the type
// checking and garbage collection of arguments.
P2_term *prim_reduce(P2_term *term)
{
    int i;
    P2_atom *arg, *return_value;
    P2_term *result;
    P2_error err = (P2_error) 0;
    void **cur = term->head + 2;
    P2_primitive *prim = ((P2_primitive *) *cur)->value;
    cur++;

    if (P2_term_length(term) <= prim->parameters)
        result = term;

    else
    {
        // Type-check the arguments, bail if there is a mismatch.
        for (i = 0; i < prim->parameters; i++)
        {
            if ((unsigned int) *cur != 2)
            {
                err = PRIMITIVE_APPLIED_TO_NONATOM;
                break;
            }
            cur++;
            arg = (P2_atom *) *cur;
            if (arg->type != prim->parameter_types[i])
            {
                err = TYPE_MISMATCH;
                break;
            }
            args[i] = arg;
            cur++;
        }

        // Apply the primitive.
        if (!err)
        {
            // I'm sure this is the stupid way to type-cast a function of a
            // variable number of parameters... but it's handy enough for now.
            switch (prim->parameters)
            {
                // Note: no 0 case.
                case 1:
                    return_value = (P2_atom *) ((void *(*)(void *)) prim->value)(
                        args[0]);
                    break;
                case 2:
                    return_value = (P2_atom *) ((void *(*)(void *, void *)) prim->value)(
                        args[0], args[1]);
                    break;
                case 3:
                    return_value = (P2_atom *) ((void *(*)(void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2]);
                    break;
                case 4:
                    return_value = (P2_atom *) ((void *(*)(void *, void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2], args[3]);
                    break;
                case 5:
                    return_value = (P2_atom *) ((void *(*)(void *, void *, void *, void *, void *)) prim->value)(
                        args[0], args[1], args[2], args[3], args[4]);
                    break;
                default:
                    err = TOO_MANY_PARAMETERS;
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
            #ifdef HIGHER_ORDER_PRIMITIVES
                result = SK_reduce(term);
            #else
                result = term;
            #endif
        }
    }

    return result;
}



P2_term *error_term(P2_error err)
{
    return P2_term__new(P2_error_atom(err), 2);
}



/*- end of file */


