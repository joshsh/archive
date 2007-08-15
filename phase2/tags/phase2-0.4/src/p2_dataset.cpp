/*+

  p2_data_set.cpp

  This library ties in the combinator semantics of SK.h with the Phase2 schema
  and the W3C XML DOM as defined in XMLDOM.h.  For use in a gSOAP client.

  Note: no support for Associations (==> or Elements) yet.

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

 *///////////////////////////////////////////////////////////////////////////////

// g++ -c p2_dataset.cpp -I../../gsoap-linux-2.7

#include "p2_dataset.h"
#include <cstdlib>  // for atoi



/** DOM_Element tag names */
#define P2_DATASET             "P2_DataSet"
#define P2_ELEMENT             "Element"
#define P2_SEQUENCE            "Sequence"
#define P2_PRIMITIVE_REFERENCE "PrimitiveReference"

#define P2_XMLNS        "P2"
#define P2_XMLNS_URL    "http://tempuri.org"



/** Dimensional values for P2_data_set dictionaries */
#define DICT_BUFFER_SIZE    100
#define DICT_EXPANSION      2.0
#define DICT_SPARSITY       2.0



/** Constructors/destructor *///////////////////////////////////////////////////



void P2_data_set::common_constructor(SEQUENCE *(*reduce0)(SEQUENCE *))
{
    reduce = reduce0;
    debug = false;

    dictionary = hash_table__new(
        DICT_BUFFER_SIZE,
        DICT_EXPANSION,
        DICT_SPARSITY,
        STRING_DEFAULTS);

    reverse_dictionary = hash_table__new(
        DICT_BUFFER_SIZE,
        DICT_EXPANSION,
        DICT_SPARSITY,
        ADDRESS_DEFAULTS);

    //all_atoms = hash_table__new(1000, 2.0, 2.0, ADDRESS_DEFAULTS);

    names = array__new(DICT_BUFFER_SIZE, DICT_EXPANSION);
}



P2_data_set::P2_data_set(SEQUENCE *(*reduce0)(SEQUENCE *))
{
    common_constructor(reduce0);
}



P2_data_set::P2_data_set(
        SEQUENCE *(*reduce0)(SEQUENCE *),
        DOM_Element *domset)
{
    common_constructor(reduce0);

    // Here, translation_table is only for atoms.
    translation_table = hash_table__new(100, 2.0, 2.0, ADDRESS_DEFAULTS);

    // Associate all atomic elements with decoded values.
    DOM_Element *curchild = DOM_firstChild(domset);

    while (curchild)
    {
        decode_atoms(curchild);
        curchild = DOM_nextSibling(curchild);
    }

    // Now decode the terms themselves.
    curchild = DOM_firstChild(domset);
    while (curchild)
    {
        SEQUENCE *term = decode(curchild);

    if ((P2_type) term->type != P2_error_type)
    {
        char *name = DOM_value(DOM_getAttributeNode(curchild, "name"));
        P2_error err = add(name, term);
        if (err)
        {
            char buffer[100];
        P2_print_error(err, buffer);
            cout << "\t>>Warning: " << buffer << " <<" << endl;
        }
    }
    else
        sequence__delete(term);

        curchild = DOM_nextSibling(curchild);
    }
    hash_table__delete(translation_table);
}



P2_data_set::~P2_data_set()
{
    // Destroy all the terms in the dictionary.
    hash_table__forall_targets(dictionary, (void (*)(void *)) sequence__delete);

    // Destroy dictionary and reverse dictionary.
    hash_table__delete(dictionary);
    hash_table__delete(reverse_dictionary);

    // Destroy term names.
    array__forall(names, free);
    array__delete(names);
}



/** XML encoding / decoding *///////////////////////////////////////////////////



/**
  Makes sure dictionary items are encoded first, so they appear at the
  top level of the XML (i.e. as children of the P2_DataSet element).
*/
void P2_data_set::encode_dict_firstpass(char *name, SEQUENCE *term)
{
    DOM_Element *el;

    // XML encode the term.
    if (term->type)
    {
        // Will be all done, except for a possible multiref id.
        el = encode(term);
    }
    else
    {
        // Don't add subterms yet.
        el = new_DOM_Element(document, P2_XMLNS_URL, P2_SEQUENCE, "");
        DOM_setAttributeNode(el, new_DOM_Attr(document, P2_XMLNS_URL, "sequenceType", "immediate"));

        hash_table__add(translation_table, (void *) term, (void *) el);
    }

    // Add the "name" attribute (all of these terms are named).
    DOM_setAttributeNode(el, new_DOM_Attr(document, P2_XMLNS_URL, "name", name));

    // Add el to data set document.
    DOM_appendChild(root, el);
}



// Encode secondary (non-dictionary) data items.
void P2_data_set::encode_dict_secondpass(char *name, SEQUENCE *term)
{
    // If a true sequence, fill in with child elements.
    if (!term->type)
    {
        DOM_Element *el = (DOM_Element *) hash_table__lookup(translation_table, (void *) term);

        int length = sequence__length(term);
        for (int i=0; i<length; i++)
        {
            DOM_Element *subterm_el = encode(sequence__get(term, i));
            DOM_appendChild(el, subterm_el);
            //el = DOM_nextSibling(el);
        }
    }
}



// XML-encode the entire data set.
DOM_Element *P2_data_set::encode(DOM_Element *document0)
{
    document = document0;
    translation_table = hash_table__new(100, 2.0, 2.0, ADDRESS_DEFAULTS);
    last_multiref_id = 0;
    root = new_DOM_Element(document, P2_XMLNS_URL, P2_DATASET, "");
    DOM_appendChild(document, root);
    hash_table__forall(dictionary, (void (*)(void *, void *)) encode_dict_firstpass);
    hash_table__forall(dictionary, (void (*)(void *, void *)) encode_dict_secondpass);
    hash_table__delete(translation_table);
    return root;
}



DOM_Element *P2_data_set::encode(DOM_Element *document0, char *name)
{
    return NULL; //...
}



/**
  Translate a P2_atom into the corresponding DOM element.

  ### Namespace issues...
*/
DOM_Element *P2_data_set::encode_atom(SEQUENCE *term)
{
    DOM_Element *new_element, *existing_element;
    DOM_Attr *existing_id, *new_id;
    P2_atom atom = (P2_atom) term->value;
    P2_type type = (P2_type) term->type;

    // If {atom} is already in the translation table (i.e. has already been
    // translated), use a SOAP-style reference node to point to the existing element.
    if ((existing_element = (DOM_Element *) hash_table__lookup(translation_table, (void *) atom)) != NULL)
    {
        // Not P2_ELEMENT... use the atom's own type-specific tag name.
        new_element = new_DOM_Element(document, P2_XMLNS_URL, P2_type_name(type), "");

        existing_id = DOM_getAttributeNode(existing_element, "id");

        // The existing element needs a multiref id by this point; if it doesn't
        // have one, create one for it.
        if (existing_id == DOM_NULL)
        {
            existing_id = new_DOM_Attr(document, NULL, "id", next_multiref_id());
            DOM_setAttributeNode(existing_element, existing_id);
        }

        // Reference the existing element.
        new_id = new_DOM_Attr(document, NULL, "ref", DOM_value(existing_id));
        DOM_setAttributeNode(new_element, new_id);
    }

    // ...otherwise, translate the term and add it to the translation table.
    else
    {
        P2_encode(atom, type, buffer);
        new_element = new_DOM_Element(document, NULL, P2_type_name(type), buffer);
        hash_table__add(translation_table, (void *) atom, (void *) new_element);
    }

    return new_element;
}



/**
  Translate a sequence into the corresponding DOM element.
*/
DOM_Element *P2_data_set::encode(SEQUENCE *term)
{
    DOM_Element *el;

    // Singleton sequence.
    if (term->type)
    {
        el = encode_atom(term);
    }

    // True sequence.
    else
    {
        el = new_DOM_Element(document, P2_XMLNS_URL, P2_SEQUENCE, "");
        int length = sequence__length(term);

        // Add child subterm elements to el.
        for (int i=0; i<length; i++)
        {
            DOM_Element *subterm_el = encode(sequence__get(term, i));
            DOM_appendChild(el, subterm_el);
        }

        DOM_setAttributeNode(el, new_DOM_Attr(document, P2_XMLNS_URL, "sequenceType", "immediate"));
    }

    return el;
}



// Note: no error-checking.
void P2_data_set::decode_atoms(DOM_Element *el)
{
    char *tagname = DOM_tagName(el);

    // For atoms, attempt to translate.
    if (strcmp(P2_SEQUENCE, tagname))
    {
    if (!DOM_getAttributeNode(el, "ref"))    // Ignore "reference" elements.
    {
            char *text = DOM_text(el);
        P2_type type = P2_type_lookup(tagname);
        P2_atom atom = P2_decode(type, text);

        void *id = (void *) el;
        DOM_Attr *id_attr = DOM_getAttributeNode(el, "id");
        if (id_attr)
            id = (void *) decode_multiref_id(DOM_value(id_attr));
        hash_table__add(translation_table, id, (void *) sequence__new((void *) type, (void *) atom));
    }
    }

    // For sequences, recurse.
    else
    {
        DOM_Element *curchild = DOM_firstChild(el);
    while (curchild)
    {
        decode_atoms(curchild);
        curchild = DOM_nextSibling(curchild);
    }
    }
}



SEQUENCE *P2_data_set::decode(DOM_Element *el)
{
    //static char buffer[1000];
    SEQUENCE *term, *subterm;

    // Simple term.
    if (strcmp(DOM_tagName(el), P2_SEQUENCE))
    {
    DOM_Attr *attr = DOM_getAttributeNode(el, "id");
    if (attr)
        term = (SEQUENCE *) hash_table__lookup(translation_table,
            (void *) decode_multiref_id(DOM_value(attr)));
    else
    {
        attr = DOM_getAttributeNode(el, "ref");
        if (attr)
        {
            term = sequence__copy((SEQUENCE *) hash_table__lookup(translation_table,
            (void *) decode_multiref_id(DOM_value(attr))));
        }
        else
        {
            //term = sequence__new((void *) P2_error_type, (void *) UNEXPECTED_VALUE);
        term = (SEQUENCE *) hash_table__lookup(translation_table,
            (void *) el);
        }
    }
    }

    // Compound term.
    else
    {
        DOM_Element *cur = DOM_firstChild(el);
        term = NULL;
        while (cur)
        {
            subterm = decode(cur);

            // Null subterm; do nothing (empty sequences are tolerated but not translated).
            if (!subterm)
            {
                //cout << "\t>> Warning: empty subterm. <<" << endl;
            }

            // Error in subterm; abort.
            else if ((P2_type) subterm->type == P2_error_type)
            {
                if (term)
                    sequence__delete(term);
                term = subterm;  // Propagate error upward.
                break;
            }

            // Add subterm.
            else
            {
                if (!term)
                    term = subterm;
                else
                    //Note: Left-associativity of terms is imposed here.
                    term = sequence__merge_la(term, subterm);
        }

            cur = DOM_nextSibling(cur);
        }
    }

    return term;
}



////////////////////////////////////////////////////////////////////////////////



HASH_TABLE *dictionary_;

// Attempt to replace all "dictionary lookup" terms (P2_identifiers) in a
// client-generated expression with immediate values.
SEQUENCE *resolve_id(SEQUENCE *atom_term)
{
    SEQUENCE *dict_term, *retval = atom_term;

    if ((P2_type) atom_term->type == P2_id_type)
    {
        P2_identifier *id = (P2_identifier *) atom_term->value;
        dict_term = (SEQUENCE *) hash_table__lookup(dictionary_, id->local_id);
        if (dict_term)
            retval = sequence__copy(dict_term);
        else
            retval = sequence__new((void *) P2_error_type, UNKNOWN_SYMBOL);
    }

    return retval;
}



/** Caution: the new term may replace an existing term in the dictionary; you'll
  need to make sure it is not garbage-collected nonetheless. */
P2_error P2_data_set::add(char *name, SEQUENCE *term)
{
    dictionary_ = dictionary;
//cout << "+ P2_data_set::add" << endl; cout.flush();
    P2_error err;
    term = sequence__copy(term);

    // Resolve IDs.
    term = sequence__replace_atoms(term, resolve_id);

    if (!term)
        err = UNKNOWN_SYMBOL;

    else
    {
        // Normalize and reduce.
        term = sequence__normalize_la(term);
        term = reduce(term);

        if (!term)
            err = REDUX_FAILURE;
        else if (term->type == P2_error_type)
            err = (P2_error) term->value;
        else
        {
            if (name)
            {
                char *s = strdup(name);

                SEQUENCE *displaced_term = (SEQUENCE *)
                    hash_table__add(dictionary, (void *) s, (void *) term);

                if (displaced_term)
                    sequence__delete(displaced_term);

                if (term->type)
                    hash_table__add(reverse_dictionary, term->value, (void *) s);

                array__enqueue(names, (void *) s);
            }
            err = P2_SUCCESS;
        }
    }
//cout << "- P2_data_set::add" << endl; cout.flush();

    return err;
}



SEQUENCE *P2_data_set::get(char *name)
{
    SEQUENCE *term = (SEQUENCE *)
        hash_table__lookup(dictionary, (void *) name);
    if (term)
        return sequence__copy(term);
    else
        return NULL;
}



/**
  Remove and deallocate a term from the data set's dictionary.
*/
void P2_data_set::remove(char *name)
{
    SEQUENCE *term = (SEQUENCE *)
        hash_table__remove(dictionary, (void *) name);
    if (term)
        sequence__delete(term);
}



/*
SK_term *P2_data_set::lookup(char *name)
{
    return SK_copy((SK_term *) hash_table__lookup(dictionary, (void *) name));
}
*/



/** Multiref IDs *///////////////////////////////////////////////////////////////



char *P2_data_set::next_multiref_id()
{
    static char buffer[20];
    sprintf(buffer, "ref-%d", ++last_multiref_id);
    //return strdup(buffer);
    return buffer;
}



int P2_data_set::decode_multiref_id(char *id)
{
    return atoi(id + 4);
}



/** Memory management *//////////////////////////////////////////////////////////




P2_error P2_data_set::mark_all()
{
    // For all terms in the dictionary, traverse the term, "marking" each atom in
    // all_atoms_ with a negative type index.
    hash_table__forall_targets(dictionary, (void (*)(void *)) sequence__mark);

    return P2_SUCCESS;
}



/** Debug stuff *///////////////////////////////////////////////////////////////



void P2_data_set::debug_print(SEQUENCE *term)
{
    if (debug)
    {
        printf("--> ");
        print(term);
        printf("\n");
    }
}



void P2_data_set::display(char *name)
{
//cout << "+ P2_data_set::display" << endl; cout.flush();

    static char print_buffer[100];

    SEQUENCE *term = (SEQUENCE *) hash_table__lookup(dictionary, (void *) name);
    if (term)
    {
        if (term->type)
        {
            printf("<%s>", P2_type_name((P2_type) term->type));
            printf(" %s:\t", name);
            P2_encode((P2_atom) term->value, (P2_type) term->type, print_buffer);
            printf(print_buffer);
        }
        else
        {
            printf("<%s_%s>", P2_XMLNS, P2_SEQUENCE);
            printf(" %s:\t", name);
            int length = sequence__length(term);
            print(sequence__peek(term));

            for (int i=1; i<length; i++)
            {
                printf(" ");
                print(sequence__get(term, i));
            }
        }
    }
//cout << "- P2_data_set::display" << endl; cout.flush();
}



//HASH_TABLE *dictionary_;
void display_entry(char *name, SEQUENCE *term)
{
    P2_type type = (P2_type) term->type;
    if (!type)
        cout << "    <" << P2_SEQUENCE << "> ";
    else
        cout << "    <" << P2_type_name(type) << "> ";
    cout << name << endl;
}
void P2_data_set::display_dictionary()
{
    cout << "Dictionary:" << endl;
    hash_table__forall(dictionary, (void (*)(void *, void *)) display_entry);
}



void P2_data_set::print(SEQUENCE *term)
{
  static char print_buffer[100];
//cout << "+ P2_data_set::print" << endl; cout.flush();
  if (term->type)
  {
      if (term->value)
      {
          char *name = (char *) hash_table__lookup(reverse_dictionary, term->value);
          if (name != NULL)
              printf(name);
          else
          {
              P2_encode((P2_atom) term->value, (P2_type) term->type, print_buffer);
              printf(print_buffer);
          }
      }
      else
      {
          printf(P2_type_name((P2_type) term->type));
      }
  }
  else
  {
      printf("(");
      int length = sequence__length(term);
      print(sequence__peek(term));
      for (int i=1; i<length; i++) {
        printf(" ");
        print(sequence__get(term, i));
      }
      printf(")");
  }
//cout << "- P2_data_set::print" << endl; cout.flush();
}


/*
void display_last()
{
    if (last_term)
        display(last_term);
}*/



/*- end of file */
