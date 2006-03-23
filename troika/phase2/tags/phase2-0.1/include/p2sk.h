/*+

  P2SK.h


  author: Joshua Shinavier
  last edited: 5/6/05

 *//////////////////////////////////////////////////////////////////////////////



/*
#######################################
        IMPORTANT

References to primitives in DataSets should not be full-blown PrimitiveFunctions
(complete with parameter types, etc.) -- you must change XMLPLI and lang.itf.xsd
to include a PrimitiveReference type or some such.  Then the primitive is simply
referred to by name.
########################################
*/



extern "C"
{
#include "../SK/SK.h"
#include "XMLDOM.h"
#include "util/hash_table.h"
}

#include <cstring>  //? is atoi() in here?
using namespace std;


#define P2_XMLNS		strdup("P2")
#define P2_ELEMENT		strdup("Element")
#define P2_SEQUENCE 	strdup("Sequence")
#define P2_S_COMBINATOR	strdup("S")
#define P2_K_COMBINATOR strdup("K")



////////////////////////////////////////////////////////////////////////////////

struct P2SK_type_interface
{
	ELEMENT *(*translate_SK_to_P2)(void *, HASH_TABLE *);
	void *(*translate_P2_to_SK)(ELEMENT *, HASH_TABLE *);
	void (*destroy)(void *);
	void *(*clone)(void *);
};




/*

  ? Do multiref ids need a namespace prefix?

  * No need to consider empty or singleton compound terms, as they should not
    occur within a proper expression.

  * No need to consider multireferencing for compound terms as SK terms are
    strictly tree-like data structures.  However, it is convenient to run them
    through the same translator as all other terms.

*/

class P2SK_translator
{
    const char *ELEMENT_baseName = "Element";
    const char *namespace_prefix = "P2";

	HASH_TABLE *atom_translators_by_data_type;

    // Note: an id of "ref-0" never actually occurs; the first id used is "ref-1".
    int last_multiref_id = 0;

    char *next_multiref_id();
    int read_multiref_id(char *id);

    ELEMENT *translate_atom(SK_atom *atom);

    ELEMENT *translate_term(SK_term *term);

public:

    P2SK_translator()
    {

	}

    ELEMENT *translate([non-dom DataSet])
    {
	    HASH_TABLE *translation_table = hash_table__new(...



        free(translation_table);
	}
};



/*+ SK to DOM translators */////////////////////////////////////////////////////



/**
  Translate an SK atom into the corresponding DOM element.
*/
ELEMENT *P2SK_translator::translate_atom(SK_atom *atom)
{
    ELEMENT *new_element, *existing_element;
    ATTRIBUTE *id;
    ATOM_VALUE *atomval = atom->value;

    // If {atom} is already in the translation table (i.e. has already been
    // translated), use a SOAP-style reference node to point to the existing element.
    if ((existing_element = (ELEMENT *) hash_table__lookup(translation_table, (void *) atomval)) != NULL)
    {
		new_element = NEW_ELEMENT(P2_XMLNS, P2_ELEMENT);

		existing_id = existing_element->getAttributeNode();

		// The existing element needs a multiref id by this point; if it doesn't
		// have one, create one for it.
        if (existing_id == XML_NULL)
        {
			existing_id = NEW_ATTRIBUTE(NULL, "id", next_multiref_id());
			existing_element->setAttributeNode(existing_id);
		}

        // Reference the existing element.
        new_id = NEW_ATTRIBUTE(NULL, "id", strdup(existing_id->value));
        new_element->setAttributeNode(new_id);
    }

    // ...otherwise, translate the term and add it to the translation table.
    else
    {
        new_element = ((void (*)(void *)) hash_table__lookup(atom_translators_by_data_type, (void *) atom->type))(atomval);
        hash_table__add(translation_table, (ATOM_VALUE *) atomval);
    }

    return new_element;
}



/**
  Translate an SK term into the corresponding DOM element.
*/
ELEMENT *P2SK_translator::translate_term(SK_term *term)
{
    ELEMENT *new_element;
    ATTRIBUTE *attr;

    switch (term->type)
    {
        case atom_type:

            return translate_atom(term->value.atom_value);

            break;

        case primitive_type:

            // All we need is the name; the details of the SK_primitive should
            // agree 1:1 with those of the corresponding XMLPLI:PrimitiveFunction
            // defined in the appropriate XMLPLI:LanguageInterface.
			new_element = NEW_ELEMENT(P2_XMLNS, P2_PRIMITIVE_REFERENCE);
			attr = NEW_ATTRIBUTE(NULL, strdup("name"), strdup(term->value.primitive_value->name));

            break;

        case S_type:

            new_element = NEW_ELEMENT(P2_XMLNS, P2_S_COMBINATOR);

            break;

        case K_type:

            new_element = NEW_ELEMENT(P2_XMLNS, P2_K_COMBINATOR);

            break;

        case compound_type:

            new_element = NEW_ELEMENT(P2_XMLNS, P2_SEQUENCE);
            attr = NEW_ATTRIBUTE(NULL, strdup("sequenceType"), strdup("immediate"));
			new_element->setAttributeNode(attr);

            ARRAY *term_array = term->value.array_value;
            int size = term_array->size;

            for (int i=0; i<size; i++)
                new_element->appendChild(translate_term((SK_term *) array__get(term_array, i));

            break;
    }

    return new_element;
}



/*-*//*+ Multiref IDs */////////////////////////////////////////////////////////



char *P2SK_translator::next_multiref_id()
{
    static char buffer[20];
    sprintf(buffer, "ref-%d", ++last_multiref_id);
    return strdup(buffer);
}



int P2SK_translator::read_multiref_id(char *id)
{
    return atoi(id + 4);
}



/*-*/

/*- end of file */

