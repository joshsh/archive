/*+

  sk.c

  A simple library to represent and reduce impure S,K terms.  Completely
  type-safe.

  Caution: errors occurring at the level of the imported primitives should be
  dealt with at that level.  This library contains only rudimentary exception
  handling to catch a few potential logical errors and programming mistakes.

  last edited: 6/10/05

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
#include <stdlib.h>  // For the NULL macro.



SEQUENCE *SK_S, *SK_K;

/*
SEQUENCE *SK_S__decode(char *buffer)
{
    return sequence__copy(SK_S);
}
struct P2_primitive *SK_K__decode(char *buffer)
{
    return sequence__copy(SK_K);
}*/
#define SK_S__decode NULL
#define SK_K__decode NULL



void (*register_constant_)(SEQUENCE *);
void (*debug_print_)(SEQUENCE *);

P2_error SK_init(void (*register_constant)(SEQUENCE *),
    void (*debug_print)(SEQUENCE *))
{
    register_constant_ = register_constant;
    debug_print_ = debug_print;
    
    SK_S = sequence__new((void *) SK_S_type, NULL);
    SK_K = sequence__new((void *) SK_K_type, NULL);
    //~
    SK_S_type = P2_register_type("S", NULL, SK_S__decode, NULL, NULL);
    SK_K_type = P2_register_type("K", NULL, SK_K__decode, NULL, NULL);

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



/**
  Reduce an S,K term recursively according to the rules of combinator
  expressions.  If an error is encountered during reduction, the returned
  SEQUENCE will contain a single P2_typed_constant of type P2_error_type and
  with an error value describing the exception.
  Caution: SK_reduce makes no attempt to detect infinite loops or memory
  faults due to extremely large S,K terms.
*/
SEQUENCE *SK_reduce(SEQUENCE *term)
{
    int i;
    SEQUENCE *function, *result = NULL;
    SEQUENCE *arg, *arg_x, *arg_y, *arg_z;
    struct P2_primitive *prim;
    P2_atom atom;
    P2_type type;
    //DEBUG cout << "Reducing..." << endl; cout.flush();

    // Iterate while result == NULL.
    do
    {
        // temporary...
        //debug_print_(term);

        //P2_print((void *) term);

        // Stop if the expression has been reduced to a simple term.
        if (sequence__length(term) == 1)
        {
            result = term;
            break;
        }
        //DEBUG cout << "length = " << sequence__length(term) << endl; cout.flush();

        // Get the atom at the head of the term.
        function = sequence__peek(term);
        //DEBUG cout << "A" << endl; cout.flush();

        // Attempt to apply the leftmost atom to the arguments on its right.
        //switch ((int) function->type)
        //{
        type = (P2_type) function->type;
        //DEBUG cout << "B" << endl; cout.flush();

        // If there are enough arguments, attempt to execute the primitive.
        // It's probably worth trying to find a way to consolidate the type
        // checking and garbage collection of arguments.
        if (type == P2_primitive_type)
        {
            //DEBUG cout << "primitive" << endl; cout.flush();
            prim = (struct P2_primitive *) function->value;

            // If not enough arguments, abandon reduction.
            if (sequence__length(term) <= prim->parameters)
                result = term;

            else
            {
                // Type-check the arguments, bail if there is a mismatch.
                for (i=1; i <= prim->parameters; i++)
                {
                    if ((arg = sequence__get(term, i))->type != prim->parameter_types[i-1])
                    {
                        if (!arg->type)
                            //! You need a constructor for errors, to avoid redundancy.
                            result = sequence__new((void *) P2_error_type, (void *) PRIMITIVE_APPLIED_TO_NONATOM);
                        //else if ((P2_type) arg->type == P2_error_type)
                            //result = sequence__pop(term);
                        else
                            result = sequence__new((void *) P2_error_type, (void *) TYPE_MISMATCH);
                        sequence__delete(term);
                        break;
                    }
                }

                // Apply the primitive.
                if (!result)
                {
                    // I'm sure this is the stupid way to type-cast a function of a
                    // variable number of parameters... but it's handy enough for now.
                    #define PARAM(index)    (void *) sequence__get(term, index)->value
                    switch (prim->parameters)
                    {
                        // Note: no 0 case.
                        case 1:
                            atom = (P2_atom) ((void *(*)(void *)) prim->value)(
                                PARAM(1));
                            break;
                        case 2:
                            atom = (P2_atom) ((void *(*)(void *, void *)) prim->value)(
                                PARAM(1), PARAM(2));
                            break;
                        case 3:
                            atom = (P2_atom) ((void *(*)(void *, void *, void *)) prim->value)(
                                PARAM(1), PARAM(2), PARAM(3));
                            break;
                        case 4:
                            atom = (P2_atom) ((void *(*)(void *, void *, void *, void *)) prim->value)(
                                PARAM(1), PARAM(2), PARAM(3), PARAM(4));
                            break;
                        case 5:
                            atom = (P2_atom) ((void *(*)(void *, void *, void *, void *, void *)) prim->value)(
                                PARAM(1), PARAM(2), PARAM(3), PARAM(4), PARAM(5));
                            break;
                        default:
                            atom = NULL;
                            result = sequence__new((void *) P2_error_type, (void *) TOO_MANY_PARAMETERS);
                            sequence__delete(term);
                    }
                    #undef PARAM
                    if (!result)
                    {
                        // Delete function. (!)
                        sequence__delete(function);

                        // Delete arguments. (!)
                        for (i=1; i <= prim->parameters; i++)
                            sequence__delete(sequence__get(term, i));

                        // Replace function and arguments with result;
                        arg = sequence__new((void *) prim->return_type, (void *) atom);
                        term = sequence__replace_head_la(term, prim->parameters+1, arg);
                    }
                }
            }
        }

            // Sxyz --> xz(yz)
            // Note: the S combinator will duplicate atoms and nratoms indiscriminately.
            else if (type == SK_S_type)
            {
            //case (int) SK_S_type:
                //DEBUG cout << "S..." << endl; cout.flush();
                // If not enough arguments, abandon reduction.
                if (sequence__length(term) < 4)
                    result = term;

                else
                {
                    //DEBUG cout << "S ^ reduce" << endl; cout.flush();

                    arg_x = sequence__get(term, 1);
                    arg_y = sequence__get(term, 2);
                    arg_z = sequence__get(term, 3);

                    arg = SK_reduce(sequence__merge_la(

                        // {xz}
                        SK_reduce(sequence__merge_la(arg_x, sequence__copy(arg_z))),

                        // {yz}
                        SK_reduce(sequence__merge_la(arg_y, arg_z))

                        ));//*/

                    term = SK_reduce(sequence__replace_head_la(term, 4, arg));

                    // Throw away the S combinator term {function}.
                    sequence__delete(function);
                }
                //break;
            }

            // Kxy --> x
            else if (type == SK_K_type)
            {
            //case (int) SK_K_type:
                //DEBUG cout << "K" << endl; cout.flush();
                // If not enough arguments, abandon reduction.
                if (sequence__length(term) < 3)
                    result = term;

                else
                {
                    // Throw away the second argument (what would be arg_y), as per the
                    // reduction rule of K.
                    sequence__delete(sequence__get(term, 2));

                    term = sequence__replace_head_la(term, 3, sequence__get(term, 1));

                    // Throw away the K combinator term {function}.
                    sequence__delete(function);
                }
                //break;

            /*case P2_error_type:

                ...
                break;*/

            // The leftmost subterm should NOT contain sub-elements, as the array
            // representation of terms is left-associative.
            }

            else if (!type)
            {
            //case (int) TERM_TYPE:

                result = sequence__new((void *) P2_error_type, (void *) PROGRAM_ERROR);
                sequence__delete(term);
                //break;

            // Any atom which is not an S or K combinator or a P2_primitive is
            // is considered a non-redex atom (and these cannot be used as
            // functions).
            }

            else
            {
            //default:
                //DEBUG cout << "default" << endl; cout.flush();

                #ifdef PERMIT_IRREDUCIBLE_TERMS
                    result = term;
                #else
                    // Garbage-collect whatever is left of {term}
                    sequence__delete(term);

                    // Generate an error.
                    result = sequence__new((void *) P2_error_type, (void *) ATOM_APPLIED_AS_FUNCTION);

                #endif
        }

    } while (!result);
    //DEBUG cout << "Done reducing " << endl; cout.flush();

    return result;
}



/*- end of file */
