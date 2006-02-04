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

#ifndef XMLDOM_H
#define XMLDOM_H


typedef void *dom_attr, *dom_document, *dom_element, *dom_namespace ;


/** Performs any operations necessary to initialize the XML module. */
void xmldom__init( ) ;

/** Performs any operations necessary to reclaim memory allocated by the XML
    module. */
void xmldom__end( ) ;


// dom_attr ////////////////////////////////////////////////////////////////////

/** \param el  element to which to attach the attribute
    \param name  name of the attribute
    \param value  value of the attribute
    \param ns  associated namespace of the attribute (optional)
    \return  a new attribute */
dom_attr *dom_attr__new( dom_element *el,
                         unsigned char *name,
                         unsigned char *value,
                         dom_namespace *ns ) ;

/** Frees an attribute.
    \param attr  reference attribute */
void dom_attr__delete( dom_attr *attr ) ;

/** \param attr  reference attribute
    \return  name of the attribute */
const unsigned char *dom_attr__name( dom_attr *attr ) ;

/** \param attr  reference attribute
    \return  value of the attribute */
const unsigned char *dom_attr__value( dom_attr *attr ) ;

/** Gets the attribute's namespace (if any).
    \param attr  reference attribute  */
dom_namespace *dom_attr__namespace( dom_attr *attr ) ;

/** Gets the attribute's next sibling (if any).
    \param attr  reference attribute  */
dom_attr *dom_attr__next_sibling( dom_attr *attr ) ;


// dom_document ////////////////////////////////////////////////////////////////

/** \return  a new document */
dom_document *dom_document__new( ) ;

/** Frees a document.
    \param doc  reference document */
void dom_document__delete( dom_document *doc ) ;

/**  \param doc  reference document
     \return  root element of the document */
dom_element *dom_document__root( dom_document *doc ) ;

/** Sets the root element of a document.
    \param doc  reference document
    \param root  the element to add as root */
void dom_document__set_root( dom_document *doc, dom_element *root ) ;

/** Outputs a document as serialized XML.
    \param doc  reference document
    \param path  system id of a file to which to write the document */
void dom_document__write_to_file( dom_document *doc, char *path ) ;

/** \param path  system id of a file from which to read the XML-encoded document
    \return  the deserialized document */
dom_document *dom_document__read_from_file( char *path ) ;


// dom_element /////////////////////////////////////////////////////////////////

/** \param doc  parent document.  Not needed by libxml2, but retained for
    compatibility with gSOAP.
    \param name  name of the element
    \param ns  associated namespace (optional)
    \return  a new element */
dom_element *dom_element__new( dom_document *doc,
                               unsigned char *name,
                               dom_namespace *ns ) ;

/** Frees an element.
    \param el  reference element */
void dom_element__delete( dom_element *el ) ;

/** \param el  reference element
    \return  name of the element */
const unsigned char *dom_element__name( dom_element *el ) ;

/** \param el  reference element
    \return  the element's namespace (if any) */
dom_namespace *dom_element__namespace( dom_element *el ) ;

/** \param el  reference element
    \return  text contained within the element */
const unsigned char *dom_element__text( dom_element *el ) ;

/** Associates the element with a namespace.
    \param el  reference element
    \param ns  namespace to associate */
void dom_element__set_namespace( dom_element *el, dom_namespace *ns );

/** \param el  reference element
    \return  the element's first child element (if any) */
dom_element *dom_element__first_child( dom_element *el ) ;

/** \param el  reference element
    \return  the element's first attribute (if any) */
dom_attr *dom_element__first_attr( dom_element *el ) ;

/** \param el  reference element
    \return  the element's next sibling (if any) */
dom_element *dom_element__next_sibling( dom_element *el ) ;

/** Adds a child element.
    \param el  reference element
    \param child  the child element */
void dom_element__add_child( dom_element *el, dom_element *child ) ;

/** Adds a text node to a parent element.
    \param el  reference element
    \param text  the text to add */
void dom_element__add_text( dom_element *el, unsigned char *text ) ;

/** \param el  reference element
    \param attr_name  name of the target attribute
    \param namespace_uri  associated namespace of the target attribute
    (optional)
    \return  the target attribute, or NULL if not found */
dom_attr *dom_element__attr( dom_element *el,
                             unsigned char *attr_name,
                             unsigned char *namespace_uri ) ;


// dom_namespace ///////////////////////////////////////////////////////////////

/** \param el  element to which to attach the namespace attribute
    \param href  URI reference
    \param prefix  namespace alias for QNames
    \return  a new namespace */
dom_namespace *dom_namespace__new( dom_element *el,
                                   unsigned char *href,
                                   unsigned char *prefix ) ;

/** Frees a namespace.
    \param ns  reference namespace */
void dom_namespace__delete( dom_namespace *ns ) ;

/** \param ns  reference namespace
    \return  namespace alias for QNames */
const unsigned char *dom_namespace__prefix( dom_namespace *ns ) ;

/** \param ns  reference namespace
    \return  URI reference */
const unsigned char *dom_namespace__href( dom_namespace *ns ) ;


#endif  // XMLDOM_H

