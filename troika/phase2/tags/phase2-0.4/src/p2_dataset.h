/*+

  p2_data_set.h

  This library ties in the combinator semantics of SK.h with the Phase2 schema
  and the W3C XML DOM as defined in XMLDOM.h.  For use in a gSOAP client.

  Note: no support for Associations (==> or Elements) yet.

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

 *//////////////////////////////////////////////////////////////////////////////

/*

  ? multiref ids shouldn't need a namespace prefix, right?

  * No need to consider empty or singleton compound terms, as they should not
    occur within a proper expression.

  * No need to consider multireferencing for compound terms as SK terms are
    strictly tree-like data structures.

*/

#ifndef P2_DATASET_H
#define P2_DATASET_H	1



extern "C"
{
    #include "p2.h"

    #include "util/array.h"
    //#include "util/set.h"
    #include "util/hash_table.h"
    #include "util/sequence.h"
}

#include "xml/xmldom.h"



class P2_data_set
{
    HASH_TABLE *dictionary;
    HASH_TABLE *reverse_dictionary;
    ARRAY *names;

    // A complete set of data elements for "last moment sweep" garbage
    // collection (each atom is associated with its type).
    //HASH_TABLE *all_atoms;
    //SET *all_atoms;

    SEQUENCE *(*reduce)(SEQUENCE *);

    void common_constructor(SEQUENCE *(*reduce0)(SEQUENCE *));

    static char *next_multiref_id();
    static int decode_multiref_id(char *id);

    static DOM_Element *encode(SEQUENCE *term);
    static SEQUENCE *decode(DOM_Element *domterm);
    static DOM_Element *encode_atom(SEQUENCE *term);
    static void decode_atoms(DOM_Element *el);

    static void encode_dict_firstpass(char *name, SEQUENCE *term);
    static void encode_dict_secondpass(char *name, SEQUENCE *term);
    //*/

public:

    // Must be public for now.
    bool debug;

    // temporary...
    void debug_print(SEQUENCE *term);
    void print(SEQUENCE *term);
    void display(char *name);
    void display_dictionary();
    //void display_last();

    P2_data_set(SEQUENCE *(*reduce0)(SEQUENCE *));

    P2_data_set(
        SEQUENCE *(*reduce0)(SEQUENCE *),
        DOM_Element *domset);

    ~P2_data_set();

    DOM_Element *encode(DOM_Element *document0);

    DOM_Element *encode(DOM_Element *document0, char *name);

    P2_error add(char *name, SEQUENCE *term);

    SEQUENCE *get(char *name);

    void remove(char *name);

    P2_error mark_all();

    //SK_term *lookup(char *name);

};



// Global variables and for callback from non-static member functions.
// These are only meaningful during XML encoding or decoding.

    DOM_Element *document, *root;
    HASH_TABLE *translation_table;
    char buffer[1000];

    // Note: an id of "ref-0" never actually occurs; the first id used is "ref-1".
    int last_multiref_id;



#endif

/*- end of file */
