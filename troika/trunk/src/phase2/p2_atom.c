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

#include "p2_atom.h"

#include <stdlib.h>  // malloc



#ifdef P2FLAGS__MARK_AND_SWEEP

    #include "util/p2_bunch.h"

    p2_bunch *markandsweep_atoms = 0;

#endif



p2_error p2_atom_init()
{
    #ifdef P2FLAGS__MARK_AND_SWEEP
    // Initialize "mark and sweep" array.
    markandsweep_atoms = p2_bunch__new(1000);
    #endif

    return P2_SUCCESS;
}



p2_error p2_atom_end()
{
    #ifdef P2FLAGS__MARK_AND_SWEEP
    p2_sweep();

    if (markandsweep_atoms)
    {
        p2_bunch__delete(markandsweep_atoms);
        markandsweep_atoms = 0;
    }
    #endif

    return P2_SUCCESS;
}



p2_atom *p2_atom__new(p2_type type, void *value)
{
    p2_atom *atom = (p2_atom *) malloc(sizeof(p2_atom));
    atom->type = type;
    atom->value = value;

    #ifdef USE_ASSOCIATION
    inbound_edges = 0;
    outbound_edges = 0;
    #endif

    #ifdef P2FLAGS__MARK_AND_SWEEP
    p2_bunch__add(markandsweep_atoms, (void *) atom);
    #endif
}



void p2_atom__delete(p2_atom *atom)
{
    #ifdef USE_ASSOCIATION
    if (atom->inbound_edges)
        p2_hash_table__delete(atom->inbound_edges);
    if (atom->outbound_edges)
        p2_hash_table__delete(atom->outbound_edges);
    #endif

    free(atom);
}



/** "Mark and sweep" memory reclamation *////////////////////////////////////////



#ifdef P2FLAGS__MARK_AND_SWEEP

p2_atom *mark(p2_atom *atom)
{
    if (atom->type > 0)
        atom->type = (p2_type) -((unsigned int) atom->type);
    return atom;
}



void p2_mark(p2_term *term)
{
    p2_p2_term__substitute_all(term, (void *(*)(void *)) mark);
}



void p2_sweep()
{
    int i, size = markandsweep_atoms->size;
    p2_atom *atom;

    for (i = 0; i < size; i++)
    {
        atom = (p2_atom *) p2_array__get(markandsweep_atoms, i);
        if (atom->type > 0)
        {
            p2_array__simple_remove(markandsweep_atoms, i);
            size--;
            i--;
            deallocate_atom(atom);
            free(atom);
        }
        else
            // un-mark
            atom->type = (p2_type) -((unsigned int) atom->type);
    }
}



unsigned int p2_total_markandsweep_atoms()
{
   if (markandsweep_atoms)
        return markandsweep_atoms->size;
   else
        return 0;
}



#endif  // P2FLAGS__MARK_AND_SWEEP


