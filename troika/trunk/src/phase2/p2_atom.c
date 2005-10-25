#include "p2_atom.h"

#include <stdlib.h>  // malloc



#ifdef P2DEF_MARK_AND_SWEEP

    #include "util/bunch.h"

    P2_bunch *markandsweep_atoms = 0;

#endif



P2_error P2_atom_init()
{
    #ifdef P2DEF_MARK_AND_SWEEP
    // Initialize "mark and sweep" array.
    markandsweep_atoms = array__new(1000, 2.0);
    #endif

    return P2_SUCCESS;
}



P2_error P2_atom_end()
{
    #ifdef P2DEF_MARK_AND_SWEEP
    P2_sweep();

    if (markandsweep_atoms)
    {
        array__delete(markandsweep_atoms);
        markandsweep_atoms = NULL;
    }
    #endif

    return P2_SUCCESS;
}



P2_atom *P2_atom__new(P2_type type, void *value)
{
    P2_atom *atom = (P2_atom *) malloc(sizeof(P2_atom));
    atom->type = type;
    atom->value = value;

    #ifdef USE_ASSOCIATION
    inbound_edges = NULL;
    outbound_edges = NULL;
    #endif

    #ifdef P2DEF_MARK_AND_SWEEP
    markandsweep_atoms->add((void *) atom);
    #endif
}



void P2_atom__delete(P2_atom *atom)
{
    #ifdef USE_ASSOCIATION
    if (atom->inbound_edges)
        hash_table__delete(atom->inbound_edges);
    if (atom->outbound_edges)
        hash_table__delete(atom->outbound_edges);
    #endif

    free(atom);
}



/** "Mark and sweep" memory reclamation *////////////////////////////////////////



#ifdef P2DEF_MARK_AND_SWEEP

P2_atom *mark(P2_atom *atom)
{
    if (atom->type > 0)
        atom->type = (P2_type) -((unsigned int) atom->type);
    return atom;
}



void P2_mark(P2_term *term)
{
    P2_term__substitute_all(term, (void *(*)(void *)) mark);
}



void P2_sweep()
{
    int i, size = markandsweep_atoms->size;
    P2_atom *atom;

    for (i = 0; i < size; i++)
    {
        atom = (P2_atom *) array__get(markandsweep_atoms, i);
        if (atom->type > 0)
        {
            array__simple_remove(markandsweep_atoms, i);
            size--;
            i--;
            deallocate_atom(atom);
            free(atom);
        }
        else
            // un-mark
            atom->type = (P2_type) -((unsigned int) atom->type);
    }
}



unsigned int P2_total_markandsweep_atoms()
{
   if (markandsweep_atoms)
        return markandsweep_atoms->size;
   else
        return 0;
}



#endif  // P2DEF_MARK_AND_SWEEP


