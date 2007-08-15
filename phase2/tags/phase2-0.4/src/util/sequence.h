/*+
  sequence.h

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


  last edited: 6/4/05

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

#ifndef SEQUENCE_H
#define SEQUENCE_H



/**
    This macro deals with deallocation of the atomic values nested in a
    JS_sequence.  If defined, the passive deallocator sequence__delete (which
    only cleans up a term itself, not its leaf elements) disappears and is
    replaced by its reference-counting counterpart sequence__delete_rc, which
    requires a once-per-term pointer to the deallocating function for atoms.
    It's important to be sure whether this is the right memory reclamation scheme
    for your application -- otherwise you could end up with program failure or
    memory leaks.
*/
//#define REFERENCE_COUNTING	1

/*
   Caution: at most one of these macros should be defined at a time.
*/
#define MARK_AND_SWEEP	1



// Note: giving every compound term a useless reference_count is not very
// efficient, although for larger sequences it will hardly be significant.
struct JS_sequence
{
    void *type;
    void *value;
    #ifdef REFERENCE_COUNTING
        int reference_count;
    #endif
};

#define SEQUENCE struct JS_sequence



// Note: these are only necessary if you are using memory reclamation.
void sequence__init(void (*deallocate_atom)(void *, void *));
void sequence__end();


// Constructor
SEQUENCE *sequence__new(void *type, void *value);

// Copy constructor
SEQUENCE *sequence__copy(SEQUENCE *S);

// Destructor
void sequence__delete(SEQUENCE *S);


// To deallocate ALL atoms, call sequence__sweep without first calling sequence__mark.
// This is also necessary to deallocate the hash table in sequence.c.
// Note: a series of calls to sequence__mark must be immediately followed by a
// call to sequence__sweep (i.e. before any new atoms are created or destroyed),
// otherwise the all_atoms_ table will not be accurate, and errors or leaks will result.
#ifdef MARK_AND_SWEEP
    void sequence__mark(SEQUENCE *S);

    void sequence__sweep();

    int sequence__all_atoms();
#endif


int sequence__length(SEQUENCE *S);

void *sequence__type(SEQUENCE *S);

void *sequence__value(SEQUENCE *S);

// Fill these out to include all of the JS_array "list" functions.
SEQUENCE *sequence__get(SEQUENCE *S, int index);
// Note: returns the displaced element, NOT the parent sequence.
SEQUENCE *sequence__set(SEQUENCE *S, int index, SEQUENCE *T);

SEQUENCE *sequence__peek(SEQUENCE *a);

SEQUENCE *sequence__replace_head_la(SEQUENCE *S, int n, SEQUENCE *T);

// Temporary...
//SEQUENCE *sequence__pop(SEQUENCE *a);
//SEQUENCE *sequence__dequeue(SEQUENCE *a);


// If in doubt as to whether a sequence is in normal form...
SEQUENCE *sequence__normalize(SEQUENCE *S);
SEQUENCE *sequence__normalize_la(SEQUENCE *S);
SEQUENCE *sequence__normalize_ra(SEQUENCE *S);


// Merge sequences, preserving nesting.
SEQUENCE *sequence__merge(SEQUENCE *S, SEQUENCE *T);

// Left-associative merge.
SEQUENCE *sequence__merge_la(SEQUENCE *S, SEQUENCE *T);

// Right-associative merge.
SEQUENCE *sequence__merge_ra(SEQUENCE *S, SEQUENCE *T);

// Contatenate.
SEQUENCE *sequence__cat(SEQUENCE *S, SEQUENCE *T);


void *sequence__forall(SEQUENCE *S, void *(*f)(SEQUENCE *));
void *sequence__forall_atoms(SEQUENCE *S, void *(*f)(SEQUENCE *));

void *sequence__exists(SEQUENCE *S, void *(*f)(SEQUENCE *));
void *sequence__exists_atom(SEQUENCE *S, void *(*f)(SEQUENCE *));


// A wrapper for "free" which always returns a 1 (for use in conjunction with
// sequence__forall)
void *sequence__free(void *s);

SEQUENCE *sequence__replace_atoms(SEQUENCE *S, SEQUENCE *(*replacement_func)(SEQUENCE *));
//void sequence__swap(SEQUENCE *S, SEQUENCE *T);



#endif

/*- end of file --------------------------------------------------------------*/
