/*+
  sequence.c

  Notes and explanation...

    "Destructive" policy towards arguments to constructive functions (incorporation of arguments).
    Automatic minimal nesting.
    No empty sequences.
    Caution: no checking for null arguments.

    All functions (apart from the normalizers) which take a JS_sequence as an
    argument ASSUME that it is in normal form.  If in doubt, call
    sequence__normalize or its left-associative or right-associative variant.


  Result of applying various merge functions to sequences (A1 ... Am) and (B1 ... Bn)

      sequence__merge :     ((A1 ... Am) (B1 ... Bn))    simple merge

      sequence__merge_la :  (A1 ... Am (B1 ... Bn))      left-associative merge

      sequence__merge_ra :  ((A1 ... Am) B1 ... Bn)      right-associative merge

      sequence__cat :       (A1 ... Am B1 ... Bn)        concatenation

      
  last edited: 6/9/05

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

#include "sequence.h"
#include "array.h"

#include <stdlib.h>



#define SEQUENCE_EXPANSION    2.0
#define SEQUENCE_ALLOC        (SEQUENCE *) malloc(sizeof(SEQUENCE))



// A wrapper for "free" which always returns a 1 (for use in conjunction with
// sequence__forall)
void *sequence__free(void *s)
{
    free(s);
    return (void *) 1;
}



/** Init and end (only necessary if using memory reclamation) */////////////////



void (*deallocate_atom_)(void *, void *);

#ifdef MARK_AND_SWEEP

    ARRAY *all_atoms_ = NULL;
    void sequence__init(void (*deallocate_atom)(void *, void *))
    {
        deallocate_atom_ = deallocate_atom;
        all_atoms_ = array__new(1000, 2.0);
    }

    // Note: you need to call sequence__sweep first.
    void sequence__end()
    {
        array__delete(all_atoms_);
        all_atoms_ = NULL;
    }

    #else

    void sequence__init(void (*deallocate_atom)(void *, void *))
    {
        deallocate_atom_ = deallocate_atom;
    }

    // For consistency with the reference-counting scenario.
    void sequence__end() {}

#endif



/** Constructors and destructor *///////////////////////////////////////////////



// Constructor
SEQUENCE *sequence__new(void *type, void *value)
{
    SEQUENCE *S;

    if (type)
    {
        S = SEQUENCE_ALLOC;
        S->type = type;
        S->value = value;

        // Note: an atomic sequence is only deallocated if it is incorporated
        // into a larger sequence which is then itself deallocated.  Until then,
        // the sequence will persist with a reference count of 0.
        #ifdef REFERENCE_COUNTING
            S->reference_count = 0;
        #endif

        #ifdef MARK_AND_SWEEP
            array__enqueue(all_atoms_, (void *) S);
        #endif
    }
    else
        S = NULL;

    return S;
}



// Copy constructor
SEQUENCE *sequence__copy(SEQUENCE *S)
{
    SEQUENCE *S2;
    ARRAY *array, *array2;
    int i, size;

    // If using a memory reclamation scheme, don't really create a new atom;
    // instead reference the existing atom.
    if (S->type)
    {
        #ifdef REFERENCE_COUNTING
            S->reference_count++;
            S2 = S;
        #else
            #ifdef MARK_AND_SWEEP
                S2 = S;
            #else
                S = SEQUENCE_ALLOC;
                S2->value = S->value;
                S2->type = S->type;
            #endif
        #endif
    }

    // Garbage-collection does not apply to compound terms.
    else
    {
        S2 = SEQUENCE_ALLOC;
        S2->type = NULL;
        array = (ARRAY *) S->value;
        size = array->size;
        array2 = array__new(array->buffer_size, array->expansion);
        S2->value = (void *) array2;
        for (i=0; i<size; i++)
            array__enqueue(array2, (void *) sequence__copy(array__get(array, i)));
    }

    return S2;
}



// Destructor
void sequence__delete(SEQUENCE *S)
{
    int i, size;
    ARRAY *array;

    if (!S->type)
    {
        array = (ARRAY *) S->value;
        array__forall(array, (void (*)(void *)) sequence__delete);
        array__delete(array);
        free(S);
    }

    else
    {
        #ifdef REFERENCE_COUNTING
            S->reference_count--;
            if (S->reference_count < 1)
            {
                deallocate_atom_(S->value, S->type);
                free(S);
            }
        #else
            // If using mark and sweep approach, the atom term must not be
            // deleted (that's for the reclamation algorithms to do).
            #ifndef MARK_AND_SWEEP
                free(S);
            #endif
        #endif
    }
}



/** "Mark and sweep" memory reclamation *////////////////////////////////////////



#ifdef MARK_AND_SWEEP

    // Note: sequence__mark assumes that type identifiers are positive (it "marks"
    // atoms by reversing the sign of their type id).
    void sequence__mark(SEQUENCE *S)
    {
        int i, size;
        ARRAY *array;

        if (S->type)
        {
            if (S->type > 0)
                S->type = (void *) (- (int) S->type);
        }

        else
        {
            array = (ARRAY *) S->value;
            size = array->size;
            // It would be convenient if JS_array used the same strategy for its "forall"...
            for (i=0; i<size; i++)
                sequence__mark((SEQUENCE *) array__get(array, i));
        }
    }

//#include <stdio.h>

    void sequence__sweep()
    {
        int i, size = all_atoms_->size;
        SEQUENCE *atom_seq;

        for (i=0; i<size; i++)
        {
            atom_seq = (SEQUENCE *) array__get(all_atoms_, i);
            if ((int) atom_seq->type > 0)
            {
                array__simple_remove(all_atoms_, i);
                size--;
                i--;
                deallocate_atom_(atom_seq->value, atom_seq->type);
                free(atom_seq);
            }
            else
            {
                // un-mark
                atom_seq->type = (void *) (- (int) atom_seq->type);
            }
        }
    }

    int sequence__all_atoms()
    {
        if (all_atoms_)
            return all_atoms_->size;
        else
            return 0;
    }

#endif



/** Accessors */////////////////////////////////////////////////////////////////



int sequence__length(SEQUENCE *S)
{
    if (!S->type)
        return ((ARRAY *) S->value)->size;
    else
        return 1;
}



void *sequence__type(SEQUENCE *S)
{
    return S->type;
}



void *sequence__value(SEQUENCE *S)
{
    return S->value;
}



SEQUENCE *sequence__get(SEQUENCE *S, int index)
{
    SEQUENCE *S2;
    ARRAY *array;

    if (!S->type)
        // Note: bounds checking contained in array__get
        S2 = (SEQUENCE *) array__get((ARRAY *) S->value, index);
    else if (!index)
        S2 = S;
    else
        S2 = NULL;

    return S2;
}



// Note: returns "displaced" item.
SEQUENCE *sequence__set(SEQUENCE *S, int index, SEQUENCE *T)
{
    SEQUENCE *S2;
    ARRAY *array;

    if (!S->type)
        // Note: bounds checking contained in array__get
        S2 = (SEQUENCE *) array__set((ARRAY *) S->value, index, (void *) T);
    else if (!index)
        S2 = S;
    else
        S2 = NULL;

    return S2;
}



SEQUENCE *sequence__peek(SEQUENCE *S)
{
    SEQUENCE *S2;
    ARRAY *array;

    if (S->type)
        S2 = S;
    else
        // Note: bounds checking contained in array__get
        S2 = (SEQUENCE *) array__peek((ARRAY *) S->value);

    return S2;
}



/** List functions *////////////////////////////////////////////////////////////



// Caution: does not delete "displaced" terms; having inspected, them, you're
// expected to do that after the replacement.
// Note: assumes that 0 < n <= length of S.
SEQUENCE *sequence__replace_head_la(SEQUENCE *S, int n, SEQUENCE *T)
{
    SEQUENCE *U;
    ARRAY *array, *array2;
    int i, size, size2;

    if (S->type)
        U = T;

    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        if (n == size)
        {
            //sequence__delete(S);
            array__delete(array);
            free(S);
            U = T;
        }
        else
        {
            for (i=0; i<n; i++)
                //sequence__delete(array
                array__pop(array);

            // This is the left-associative bit...
            if (T->type)
                array__push(array, (void *) T);
            else
            {
                array2 = (ARRAY *) T->value;
                size2 = array2->size;
                for (i=0; i<size2; i++)
                    array__push(array, array__dequeue(array2));
                array__delete(array2);
                free(T);
            }

            if (array->size == 1)
            {
                U = (SEQUENCE *) array__pop(array);
                array__delete(array);
                free(S);
            }
            else
                U = S;
        }
    }

    return U;
}


/*
SEQUENCE *sequence__pop(SEQUENCE *S)
{
    SEQUENCE *S2, *S3;
    ARRAY *array;

    if (S->type)
        S2 = S;  //~ S will still appear to contain one element.
    else
    {
        array = (ARRAY *) S->value;
        S2 = (SEQUENCE *) array__pop(array);

        // Disallow single-element arrays.
        if (array->size == 1)
        {
            S3 = (SEQUENCE *) array__pop(array);
            S->type = S3->type;
            S->value = S3->value;
            free(S3);
            free(array);
        }
    }

    return S2;
}



SEQUENCE *sequence__dequeue(SEQUENCE *S)
{
    SEQUENCE *S2, *S3;
    ARRAY *array;

    if (S->type)
        S2 = S;  //~ S will still appear to contain one element.
    else
    {
        array = (ARRAY *) S->value;
        S2 = (SEQUENCE *) array__dequeue(array);

        // Disallow single-element arrays.
        if (array->size == 1)
        {
            S3 = (SEQUENCE *) array__pop(array);
            S->type = S3->type;
            S->value = S3->value;
            free(S3);
            free(array);
        }
    }

    return S2;
}
*/


/** Normalizing functions */////////////////////////////////////////////////////



SEQUENCE *sequence__normalize(SEQUENCE *S)
{
    SEQUENCE *S2, *s;
    ARRAY *array;
    int i, size;

    if (!S)
        S2 = NULL;
    else if (S->type)
        S2 = S;
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        for (i=0; i<size; i++)
        {
            s = sequence__normalize((SEQUENCE *) array__pop(array));
            if (s)
                array__enqueue(array, (void *) s);
        }
        size = array->size;
        if (!size)
        {
            S2 = NULL;
            sequence__delete(S);
        }
        else if (size == 1)
        {
            S2 = (SEQUENCE *) array__pop(array);
            sequence__delete(S);
        }
        else
        {
            S2 = S;
        }
    }
    return S2;
}



SEQUENCE *sequence__normalize_la(SEQUENCE *S)
{
    SEQUENCE *S2, *s;
    ARRAY *array, *array2;
    int i, size, size2;

    if (!S)
        S2 = NULL;
    else if (S->type)
        S2 = S;
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        for (i=0; i<size; i++)
        {
            s = sequence__normalize_la((SEQUENCE *) array__pop(array));
            if (s)
                array__enqueue(array, (void *) s);
        }
        size = array->size;
        if (!size)
        {
            S2 = NULL;
            sequence__delete(S);
        }
        else if (size == 1)
        {
            S2 = (SEQUENCE *) array__pop(array);
            sequence__delete(S);
        }
        else
        {
            S2 = S;
            s = (SEQUENCE *) array__pop(array);
            if (!s->type)
            {
                array2 = (ARRAY *) s->value;
                size2 = array2->size;
                for (i=0; i<size2; i++)
                    array__push(array, array__dequeue(array2));
                sequence__delete(s);
            }
            else
                array__push(array, s);
        }
    }
    return S2;
}



SEQUENCE *sequence__normalize_ra(SEQUENCE *S)
{
    SEQUENCE *S2, *s;
    ARRAY *array, *array2;
    int i, size, size2;

    if (!S)
        S2 = NULL;
    else if (S->type)
        S2 = S;
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        for (i=0; i<size; i++)
        {
            s = sequence__normalize_ra((SEQUENCE *) array__pop(array));
            if (s)
                array__enqueue(array, (void *) s);
        }
        size = array->size;
        if (!size)
        {
            S2 = NULL;
            sequence__delete(S);
        }
        else if (size == 1)
        {
            S2 = (SEQUENCE *) array__pop(array);
            sequence__delete(S);
        }
        else
        {
            S2 = S;
            s = (SEQUENCE *) array__dequeue(array);
            if (!s->type)
            {
                array2 = (ARRAY *) s->value;
                size2 = array2->size;
                for (i=0; i<size2; i++)
                    array__enqueue(array, array__pop(array2));
                sequence__delete(s);
            }
            else
                array__enqueue(array, s);
        }
    }
    return S2;
}



/** Merge functions *///////////////////////////////////////////////////////////



// Merge sequences, preserving nesting.
SEQUENCE *sequence__merge(SEQUENCE *S, SEQUENCE *T)
{
    SEQUENCE *U = SEQUENCE_ALLOC;
    ARRAY * U_array = array__new(2, SEQUENCE_EXPANSION);
    U->type = NULL;
    U->value = (void *) U_array;
    array__push(U_array, (void *) T);
    array__push(U_array, (void *) S);
    return U;
}



// Left-associative merge.
SEQUENCE *sequence__merge_la(SEQUENCE *S, SEQUENCE *T)
{
    ARRAY *U_array;
    SEQUENCE *U;

    if (!S->type)  // First item is a sequence.
    {
        array__enqueue((ARRAY *) S->value, (void *) T);
        U = S;
    }
    else  // First item is a leaf node.
    {
        U = SEQUENCE_ALLOC;
        U->type = NULL;
        U_array = array__new(2, SEQUENCE_EXPANSION);
        U->value = (void *) U_array;
        array__push(U_array, (void *) T);
        array__push(U_array, (void *) S);
    }

    return U;
}



// Right-associative merge.
SEQUENCE *sequence__merge_ra(SEQUENCE *S, SEQUENCE *T)
{
    ARRAY *U_array;
    SEQUENCE *U;

    if (!S->type)  // First item is a sequence.
    {
        U = SEQUENCE_ALLOC;
        U->type = NULL;
        U_array = array__new(2, SEQUENCE_EXPANSION);
        U->value = (void *) U_array;
        array__push(U_array, (void *) T);
        array__push(U_array, (void *) S);
    }
    else  // First item is a leaf node.
    {
        array__push((ARRAY *) T->value, (void *) S);
        U = T;
    }

    return U;
}



// Contatenate.
SEQUENCE *sequence__cat(SEQUENCE *S, SEQUENCE *T)
{
    int i, size_S, size_T;
    ARRAY *S_array, *T_array, *U_array;
    SEQUENCE *U;

    if (!S->type)  // First item is a sequence.
    {
        if (!T->type)  // Two true sequences.
        {
            array__enqueue((ARRAY *) S->value, (void *) T);
            U = S;
        }
        else  // True sequence and a leaf node.
        {
            S_array = (ARRAY *) S->value;
            T_array = (ARRAY *) T->value;
            size_S = S_array->size;
            size_T = T_array->size;
            if (size_T < size_S)
            {
                for (i=0; i<size_T; i++)
                    array__enqueue(S_array, array__pop(T_array));
                array__delete(T_array);
                U = S;
            }
            else
            {
                for (i=0; i<size_S; i++)
                    array__push(T_array, array__dequeue(S_array));
                array__delete(S_array);
                U = T;
            }
        }
    }
    else  // First item is a leaf node.
    {
        if (!T->type)  // Leaf node and a true sequence.
        {
            array__push((ARRAY *) T->value, (void *) S);
            U = T;
        }
        else  // Two leaf nodes.
        {
            U = SEQUENCE_ALLOC;
            U->type = NULL;
            U_array = array__new(2, SEQUENCE_EXPANSION);
            U->value = (void *) U_array;
            array__push(U_array, (void *) T);
            array__push(U_array, (void *) S);
        }
    }

    return U;
}



/** Existence functions *///////////////////////////////////////////////////////



void *sequence__forall(SEQUENCE *S, void *(*f)(SEQUENCE *))
{
    int i, size;
    ARRAY *array;

    if (S->type)
        return f(S);
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        // It would be convenient if JS_array used the same strategy for its "forall"...
        for (i=0; i<size; i++)
        {
            if (!sequence__forall(array__get(array, i), f))
                return NULL;
        }
        return f(S);
    }
}



void *sequence__forall_atoms(SEQUENCE *S, void *(*f)(SEQUENCE *))
{
    int i, size;
    ARRAY *array;

    if (S->type)
        return f(S);
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        // It would be convenient if JS_array used the same strategy for its "forall"...
        for (i=0; i<size; i++)
        {
            if (!sequence__forall(array__get(array, i), f))
                return NULL;
        }
        return (void *) 1;
    }
}



void *sequence__exists(SEQUENCE *S, void *(*f)(SEQUENCE *))
{
    int i, size;
    ARRAY *array;

    if (S->type)
        return f(S);
    else
    {
        if (f(S))
        return (void *) 1;
        array = (ARRAY *) S->value;
        size = array->size;
        // It would be convenient if JS_array used the same strategy for its "forall"...
        for (i=0; i<size; i++)
        {
            if (sequence__exists(array__get(array, i), f))
                return (void *) 1;
        }
        return NULL;
    }
}



void *sequence__exists_atom(SEQUENCE *S, void *(*f)(SEQUENCE *))
{
    int i, size;
    ARRAY *array;

    if (S->type)
        return f(S);
    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        // It would be convenient if JS_array used the same strategy for its "forall"...
        for (i=0; i<size; i++)
        {
            if (sequence__exists(array__get(array, i), f))
                return (void *) 1;
        }
        return NULL;
    }
}



/////////////////////////////////////////////////////////////////////////////////



/**
    Use this function when performing replacements, rather than altering sequence
    members "manually".
*/
/*
void sequence__swap(SEQUENCE *S, SEQUENCE *T)
{
    void *temp_value, *temp_type;
    #ifdef REFERENCE_COUNTING
        int temp_reference_count;
    #endif

    if (S && T)
    {
        temp_value = S->value;
        S->value = T->value;
        T->value = temp_value;

        temp_type = S->type;
        S->type = T->type;
        T->type = temp_type;

        #ifdef REFERENCE_COUNTING
            temp_reference_count = S->reference_count;
            S->reference_count = T->reference_count;
            T->reference_count = temp_reference_count;
        #endif
    }
}*/



/**
  Note: displaced sequences are automatically deallocated; if you want to save
  them you need to copy them in replacement_func.
  To indicate failure, replacement_func should return a NULL, which will then
  propagate upward to the original sequence.
*/
SEQUENCE *sequence__replace_atoms(SEQUENCE *S, SEQUENCE *(*replacement_func)(SEQUENCE *))
{
    int i, size;
    ARRAY *array;
    SEQUENCE *S2;

    if (S->type)
    {
        S2 = replacement_func(S);
        if (S != S2)
            sequence__delete(S);
        return S2;
    }

    else
    {
        array = (ARRAY *) S->value;
        size = array->size;
        for (i=0; i<size; i++)
        {
            S2 = sequence__replace_atoms((SEQUENCE *) array__get(array, i), replacement_func);
            if (!S2)
            {
                sequence__delete(S);
                return NULL;
            }
            else
                array__set(array, i, (void *) S2);
        }
        return S;
    }
}



/*- end of file -*/
