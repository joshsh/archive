/*+

  XMLDOM.cpp   -- consider switching to XMLDOM.c

  A collection of stubs and macros to bind standard DOM data types, properties and methods
  with the corresponding entities in a particular XML toolkit (in this case the
  gSOAP DOM parser).  By changing only the macro and stub definitions, when
  adapting an application to a new toolkit, the application code itself remains
  independent of the toolkit.

  The functions provided do not cover the entire W3C DOM standard, and may be
  expanded as needed.

  last edited: 5/10/05


[!] Use XML-style formatting (i.e. thisCapitalizationScheme) ^ same procedure names regardless of what language they are implemented in.


g++ -c xmldom.cpp -I../../gsoap-linux-2.5


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

#include "xmldom.h"
#include <stdlib.h>
#include <iostream.h>  // For the sake of DOM_print
#include <string.h>  // For wcslen



/*+

  Element members

  Note: firstChild, nextSibling, appendChild and removeChild are of the
  Microsoft XML DOM, not W3C DOM.  They're here because they're more useful than
  getElementsByTagName and NodeLists.

  Not represented:

      DOMString                 getAttribute(in DOMString name);
      void                      setAttribute(in DOMString name,
                                             in DOMString value)
                                             raises(DOMException);
      NodeList                  getElementsByTagName(in DOMString name);
      void                      normalize();

struct soap_dom_element
{ struct soap_dom_element *next;    // next sibling
  struct soap_dom_element *prnt;    // parent
  struct soap_dom_element *elts;    // first child element
  struct soap_dom_attribute *atts;  // first child attribute
  const char *nstr;                 // namespace string
  char *name;                       // element tag name
  char *data;                       // element content data (with SOAP_C_UTFSTRING flag set)
  wchar_t *wide;                    // element content data
  int type;                         // optional: serialized C/C++ data type
  void *node;                       // optional: pointer to serialized C/C++ data
  struct soap *soap;

 *//////////////////////////////////////////////////////////////////////////////

// Note: you might want to use strdup() for some of these...
// Note: namespace is not taken into account.



/**  Attr                      getAttributeNode(in DOMString name); */
DOM_Attr *DOM_getAttributeNode(DOM_Element *root, DOM_DOMString name)
{
    DOM_Attr *cur = root->atts;

    // Search for attribute with the given name.
    while (cur != DOM_NULL)
    {
        if (!strcmp(name, cur->name))
          return cur;
        cur = cur->next;
    }

    // Attribute not found.
    return DOM_NULL;
}



/**   Attr                      setAttributeNode(in Attr newAttr)
                                             raises(DOMException); */
DOM_Attr *DOM_setAttributeNode(DOM_Element *root, DOM_Attr *newAttr)
{
    DOM_Attr *cur = root->atts, *prev = DOM_NULL;

    // Cycle to the end of the attribute list.
    while (cur != DOM_NULL)
    {
        prev = cur;
        cur = cur->next;
    }

    // Insert the new attribute.
    if (prev != DOM_NULL)
      prev->next = newAttr;
    else
      root->atts = newAttr;

    return newAttr; // ...right?
}



/**   void                      removeAttribute(in DOMString name)
                                            raises(DOMException); */
DOM_Attr *DOM_removeAttribute(DOM_Element *root, DOM_DOMString name)
{
    DOM_Attr *cur = root->atts, *prev = DOM_NULL;

    // Search for attribute with the given name.
    while (cur != DOM_NULL)
    {
        if (!strcmp(name, cur->name))
            break;
        prev = cur;
        cur = cur->next;
    }
    if (cur != DOM_NULL)
    {
        // It's the first attribute in the list.
        if (prev == DOM_NULL)
            root->atts = cur->next;

        // It's within the list.
        else
            prev->next = cur->next;

        cur->next = DOM_NULL;

        // Return the removed attribute.
        return cur;
    }

    return DOM_NULL;
}



/**   readonly attribute  DOMString            tagName; */
DOM_DOMString DOM_tagName(DOM_Element *root)
{
    return root->name;
}



/* Microsoft DOM only...
DATA_TYPE DOM_dataType(DOM_Element *root)
{
    return root->type;
}
char *DOM_namespaceURI(DOM_Element *root)
{
    return root->nstr;
}
char *DOM_nodeValue(DOM_Element *root)
{
    return root->data;
}
*/



/** (Microsoft XML DOM) */
DOM_Element *DOM_firstChild(DOM_Element *root)
{
    return root->elts;
}



/** (Microsoft XML DOM) */
DOM_Element *DOM_nextSibling(DOM_Element *root)
{
    return root->next;
}



/** (Microsoft XML DOM) */
DOM_Element *DOM_appendChild(DOM_Element *root, DOM_Element *child)
{
    //root->add(*child);

    DOM_Element *cur = root->elts, *prev = DOM_NULL;

    // Cycle to the end of the child node list.
    while (cur != DOM_NULL)
    {
        prev = cur;
        cur = cur->next;
    }

    // Insert the new child.
    if (prev != DOM_NULL)
      prev->next = child;
    else
      root->elts = child;

    return child; // ...right?
}



/** (Microsoft XML DOM) */
DOM_Element *DOM_removeChild(DOM_Element *root, DOM_Element *child)
{
    DOM_Element *cur = root->elts, *prev = DOM_NULL;

    // Search for the child in the child list.
    while (cur != DOM_NULL)
    {
        if (child == cur)
            break;
        prev = cur;
        cur = cur->next;
    }
    if (cur != DOM_NULL)
    {
        // It's the first child in the list.
        if (prev == DOM_NULL)
            root->elts = cur->next;

        // It's within the list.
        else
            prev->next = cur->next;

        child->next = DOM_NULL;

        // Return the removed child.
        return cur;
    }

    child->next = DOM_NULL;
    return DOM_NULL;
}



/** improvised */
DOM_DOMString DOM_text(DOM_Element *root)
{
//cout << "tag name = " << DOM_tagName(root) << ", wide = " << (int) root->wide << endl;
//cout.flush();
   // For some bizarre reason, text data is sometimes found in the "data" field,
   // and sometimes in the "wide" (wide-character string) field.
   if (root->data)
       return root->data;
   ///*
   int maxlen;

    // temporary...
    if (root->wide && (maxlen = wcslen(root->wide) * 2))
    {
        char *s = MALLOC(maxlen);
        wcstombs(s, root->wide, maxlen);
        return s;
    }
    else
        return NULL;//*/
}



/** improvised */
DOM_Attr *DOM_firstAttribute(DOM_Element *root)
{
    return root->atts;
}



/** improvised */
DOM_Attr *DOM_nextAttribute(DOM_Attr *attr)
{
    return attr->next;
}



/*- end Element members section *///////////////////////////////////////////////
/*+

  Attribute members

  Not represented:

      readonly attribute  boolean              specified;

struct soap_dom_attribute
{ struct soap_dom_attribute *next;
  const char *nstr;
  char *name;
  char *data;
  wchar_t *wide;
  struct soap *soap;

 *//////////////////////////////////////////////////////////////////////////////

// Note: namespace is not taken into account.



/**   readonly attribute  DOMString            name; */
DOM_DOMString DOM_name(DOM_Attr *root)
{
    return root->name;
}



/**            attribute  DOMString            value; */
DOM_DOMString DOM_value(DOM_Attr *root)
{
    // temporary...
    return root->data;
    /*
    int maxlen = wcslen(root->wide) * 2;
    char *s = MALLOC(maxlen);
    wcstombs(s, root->wide, maxlen);
    return s;*/
}



/*- end Attribute members section */

////////////////////////////////////////////////////////////////////////////////


#define STROUT out

// Temporary, while gSOAP is buggy...
void DOM_print(DOM_Element *el, ofstream &out, int indent)
{
    char *tagname = DOM_tagName(el);

    DOM_Element *curchild = DOM_firstChild(el);
    DOM_Attr *curattr = DOM_firstAttribute(el);
    char *text = DOM_text(el);

    // Indention and tag name.
    for (int i=0; i<indent; i++)
        STROUT << "    ";
    STROUT << "<" << tagname;

    // Serialize attributes (if any).
    while (curattr)
    {
        STROUT << " " << DOM_name(curattr) << "=\"" << DOM_value(curattr) << "\"";
        curattr = DOM_nextAttribute(curattr);
    }

    // No child elements.
    if ((!text || !strlen(text)) && !curchild)
    {
      STROUT << "/>\n";
    }

    // Serialize text and child elements (in that arbitrary order).
    else
    {
        STROUT << ">";

    // Write text.
        if (text)
            STROUT << text;
        if (curchild)
        {
            STROUT << endl;

            // Serialize children.
            while (curchild)
            {
                DOM_print(curchild, out, indent+1);
                curchild = DOM_nextSibling(curchild);
            }

            // Close the element.
            for (int i=0; i<indent; i++)
                STROUT << "    ";
        }
        STROUT << "</" << tagname << ">\n";
    }
}



/*- end of file */
