/*+

  XMLDOM.h

  A collection of stubs and macros to bind standard DOM data types, properties and methods
  with the corresponding entities in a particular XML toolkit (in this case the
  gSOAP DOM parser).  By changing only the macro and stub definitions, when
  adapting an application to a new toolkit, the application code itself remains
  independent of the toolkit.

  The functions provided do not cover the entire W3C DOM standard, and may be
  expanded as needed.

  last edited: 5/15/05


[!] Use XML-style formatting (i.e. thisCapitalizationScheme) ^ same procedure names regardless of what language they are implemented in.

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

#ifndef XMLDOM_H
#define XMLDOM_H    1

using namespace std;



/*
#ifndef SOAP_C_UTFSTRING
    #define SOAP_C_UTFSTRING	1
#endif
*/

#include "stdsoap2.h"  // Need -I[gSOAP path] when compiling
#include <fstream>


#define DOM_NULL    NULL



/** DOM Node objects and their constructors/destructors *///////////////////////



typedef char* DOM_DOMString;
//typedef wchar_t* DOM_DOMstring_wide;

#define DOM_Element     soap_dom_element
#define DOM_Attr        soap_dom_attribute
//! For clarity you'll want to distinguish DOM_Document from DOM_Element.

//#define DOM_NamedNodeMap    soap_dom



/*
  doc :    pointer to parent element
  namespaceURL
  baseName :    "tag name" of the element
  value :     text content of the element
*/
//[!] make stubs out of these so you can check for NULLs, etc.
#define new_DOM_Element(document, namespaceURL, baseName, value) \
    new soap_dom_element((document)->soap, NULL, strdup(baseName), strdup(value))
//#define new_DOM_Element(document, namespaceURL, baseName, value)
//    new soap_dom_element((document)->soap, namespaceURL, strdup(baseName), strdup(value))

#define delete_DOM_Element(element) \
    delete element

#define new_DOM_Attr(document, namespaceURL, baseName, value) \
    new soap_dom_attribute((document)->soap, NULL, strdup(baseName), strdup(value))
//#define new_DOM_Attr(document, namespaceURL, baseName, value) 
//    new soap_dom_attribute((document)->soap, namespaceURL, strdup(baseName), strdup(value))

#define delete_DOM_Attr(attribute) \
    delete attribute



/** Element members *///////////////////////////////////////////////////////////



/**  Attr                      getAttributeNode(in DOMString name); */
DOM_Attr *DOM_getAttributeNode(DOM_Element *root, DOM_DOMString name);

/**   Attr                      setAttributeNode(in Attr newAttr)
                                             raises(DOMException); */
DOM_Attr *DOM_setAttributeNode(DOM_Element *root, DOM_Attr *newAttr);

/**   void                      removeAttribute(in DOMString name)
                                            raises(DOMException); */
DOM_Attr *DOM_removeAttribute(DOM_Element *root, DOM_DOMString name);

/**   readonly attribute  DOMString            tagName; */
DOM_DOMString DOM_tagName(DOM_Element *root);

/** (Microsoft XML DOM) */
DOM_Element *DOM_firstChild(DOM_Element *root);

/** (Microsoft XML DOM) */
DOM_Element *DOM_nextSibling(DOM_Element *root);

/** (Microsoft XML DOM) */
DOM_Element *DOM_appendChild(DOM_Element *root, DOM_Element *child);

/** (Microsoft XML DOM) */
DOM_Element *DOM_removeChild(DOM_Element *root, DOM_Element *child);

/** improvised */
DOM_DOMString DOM_text(DOM_Element *root);

/** improvised */
DOM_Attr *DOM_firstAttribute(DOM_Element *root);

/** improvised */
DOM_Attr *DOM_nextAttribute(DOM_Attr *attr);



/** Attribute members */////////////////////////////////////////////////////////



/**   readonly attribute  DOMString            name; */
DOM_DOMString DOM_name(DOM_Attr *root);

/**            attribute  DOMString            value; */
DOM_DOMString DOM_value(DOM_Attr *root);



////////////////////////////////////////////////////////////////////////////////



// Temporary, while gSOAP is buggy...
void DOM_print(DOM_Element *el, ofstream &out, int indent);



////////////////////////////////////////////////////////////////////////////////



/*
SOAP_FMAC1 int SOAP_FMAC2 soap_s2byte(struct soap*, const char*, char*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2short(struct soap*, const char*, short*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2int(struct soap*, const char*, int*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2long(struct soap*, const char*, long*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2LONG64(struct soap*, const char*, LONG64*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2float(struct soap*, const char*, float*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2double(struct soap*, const char*, double*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2unsignedByte(struct soap*, const char*, unsigned char*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2unsignedShort(struct soap*, const char*, unsigned short*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2unsignedInt(struct soap*, const char*, unsigned int*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2unsignedLong(struct soap*, const char*, unsigned long*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2ULONG64(struct soap*, const char*, ULONG64*);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2string(struct soap*, const char*, char**);
SOAP_FMAC1 int SOAP_FMAC2 soap_s2QName(struct soap*, const char*, char**);

#ifndef WITH_LEAN
SOAP_FMAC1 int SOAP_FMAC2 soap_s2dateTime(struct soap*, const char*, time_t*);
SOAP_FMAC1 char* SOAP_FMAC2 soap_s2base64(struct soap*, const unsigned char*, char*, size_t);
SOAP_FMAC1 char* SOAP_FMAC2 soap_s2hex(struct soap*, const unsigned char*, char*, size_t);
#endif

SOAP_FMAC1 const char* SOAP_FMAC2 soap_byte2s(struct soap*, char);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_short2s(struct soap*, short);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_int2s(struct soap*, int);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_long2s(struct soap*, long);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_LONG642s(struct soap*, LONG64);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_float2s(struct soap*, float);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_double2s(struct soap*, double);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_unsignedByte2s(struct soap*, unsigned char);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_unsignedShort2s(struct soap*, unsigned short);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_unsignedInt2s(struct soap*, unsigned int);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_unsignedLong2s(struct soap*, unsigned long);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_ULONG642s(struct soap*, ULONG64);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_QName2s(struct soap*, const char*);
*/

#endif  // close #ifndef XMLDOM_H

/*- end of file */
