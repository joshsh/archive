/**

\file  xmldom.h

A collection of stubs to bind DOM data types, properties and functions with the
corresponding entities in a particular XML toolkit.  To adapt the project to a
different toolkit, you need only write a new implementation file xmldom-xxx.c,
then add a Makefile target xmldom-xxx.o and swap it into the build.

\note  These functions have been chosen with ease of use in mind, and are not
representative of any particular DOM standard.

\author  Joshua Shinavier   \n
         parcour@gmail.com  \n
         +1 509 570-6990    \n */

/*******************************************************************************

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

*******************************************************************************/

#ifndef XMLDOM_H
#define XMLDOM_H


typedef void *Attr, *Document, *Element, *Namespc ;


/** Performs any operations necessary to initialize the XML module. */
void xmldom__init( ) ;

/** Performs any operations necessary to reclaim memory allocated by the XML
    module. */
void xmldom__end( ) ;


/* Attr ***********************************************************************/

/** Frees an attribute.
    \param attr  reference attribute */
void attr__delete( Attr *attr ) ;

/** \param attr  reference attribute
    \return  name of the attribute */
const unsigned char *attr__name( Attr *attr ) ;


/** Gets the attribute's namespace (if any).
    \param attr  reference attribute  */
Namespc *attr__namespace( Attr *attr ) ;

/** \param el  element to which to attach the attribute
    \param name  name of the attribute
    \param value  value of the attribute
    \param ns  associated namespace of the attribute (optional)
    \return  a new attribute */
Attr *attr__new
    ( Element *el, unsigned char *name, unsigned char *value, Namespc *ns ) ;

/** Gets the attribute's next sibling (if any).
    \param attr  reference attribute  */
Attr *attr__next_sibling( Attr *attr ) ;

/** \param attr  reference attribute
    \return  value of the attribute */
const unsigned char *attr__value( Attr *attr ) ;


/* Document *******************************************************************/


/** Frees a document.
    \param doc  reference document */
void document__delete( Document *doc ) ;

/** \return  a new document */
Document *document__new( ) ;

/** \param path  system id of a file from which to read the XML-encoded document
    \return  the deserialized document */
Document *document__read_from_file( char *path ) ;

/**  \param doc  reference document
     \return  root element of the document */
Element *document__root( Document *doc ) ;

/** Sets the root element of a document.
    \param doc  reference document
    \param root  the element to add as root */
void document__set_root( Document *doc, Element *root ) ;

/** Outputs a document as serialized XML.
    \param doc  reference document
    \param path  system id of a file to which to write the document */
void document__write_to_file( Document *doc, char *path ) ;


/* Element ********************************************************************/


/** Adds a child element.
    \param el  reference element
    \param child  the child element */
void element__add_child( Element *el, Element *child ) ;

/** Adds a text node to a parent element.
    \param el  reference element
    \param text  the text to add */
void element__add_text( Element *el, unsigned char *text ) ;

/** \param el  reference element
    \param attr_name  name of the target attribute
    \param namespace_uri  associated namespace of the target attribute
    (optional)
    \return  the target attribute, or NULL if not found */
Attr *element__attr
    ( Element *el, unsigned char *attr_name, unsigned char *namespace_uri ) ;

/** Frees an element.
    \param el  reference element */
void element__delete( Element *el ) ;

/** \param el  reference element
    \return  the element's first attribute (if any) */
Attr *element__first_attr( Element *el ) ;

/** \param el  reference element
    \return  the element's first child element (if any) */
Element *element__first_child( Element *el ) ;

/** \param el  reference element
    \return  name of the element */
const unsigned char *element__name( Element *el ) ;

/** \param el  reference element
    \return  the element's namespace (if any) */
Namespc *element__namespace( Element *el ) ;

/** \param doc  parent document.  Not needed by libxml2, but retained for
    compatibility with gSOAP.
    \param name  name of the element
    \param ns  associated namespace (optional)
    \return  a new element */
Element *element__new
    ( Document *doc, unsigned char *name, Namespc *ns ) ;

/** \param el  reference element
    \return  the element's next sibling (if any) */
Element *element__next_sibling( Element *el ) ;

/** Associates the element with a namespace.
    \param el  reference element
    \param ns  namespace to associate */
void element__set_namespace( Element *el, Namespc *ns );

/** \param el  reference element
    \return  text contained within the element */
const unsigned char *element__text( Element *el ) ;


/* Namespc ******************************************************************/


/** Frees a namespace.
    \param ns  reference namespace */
void namespc__delete( Namespc *ns ) ;

/** \param ns  reference namespace
    \return  URI reference */
const unsigned char *namespc__href( Namespc *ns ) ;

/** \param el  element to which to attach the namespace attribute
    \param href  URI reference
    \param prefix  namespace alias for QNames
    \return  a new namespace */
Namespc *namespc__new
    ( Element *el, unsigned char *href, unsigned char *prefix ) ;

/** \param ns  reference namespace
    \return  namespace alias for QNames */
const unsigned char *namespc__prefix( Namespc *ns ) ;


#endif  /* XMLDOM_H */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
