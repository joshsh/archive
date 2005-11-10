/**
    \file  xmldom.h

    A collection of stubs and macros to bind standard DOM data types, properties
    and methods with the corresponding entities in a particular XML toolkit.
    By alternating only the implementation behind this header file, when
    adapting an application to a new toolkit, the application code itself
    remains independent of the toolkit.

    The functions provided do not cover the entire W3C DOM standard, and may be
    expanded as needed.

    \note  Use XML-style formatting (i.e. thisCapitalizationScheme) ^ same
    procedure names regardless of what language they are implemented in.

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


typedef void *dom_namespace, *dom_document, *dom_element ;


// init and end ////////////////////////////////////////////////////////////////

/** Performs any operations necessary to initialize the XML module. */
void xmldom__init( ) ;

/** Performs any operations necessary to reclaim memory allocated by the XML
    module. */
void xmldom__end( ) ;


// dom_namespace ///////////////////////////////////////////////////////////////

/** Create a new namespace object. */
dom_namespace *dom_namespace__new( ) ;

/** De-allocate a namespace object. */
void dom_namespace__delete( dom_namespace *ns ) ;


// dom_document ////////////////////////////////////////////////////////////////

/** Creates a new document. */
dom_document *dom_document__new( ) ;

/** De-allocates a document. */
void dom_document__delete( dom_document *doc ) ;

/** Gets the document's root element. */
dom_element *dom_document__root( dom_document *doc ) ;

/** Sets the root element of a document. */
void dom_document__set_root( dom_document *doc, dom_element *root ) ;

/** Outputs a document as serialized XML. */
void dom_document__write_to_file( dom_document *doc, char *path ) ;

/** Deserializes an XML DOM document from a file. */
dom_document *dom_document__read_from_file( char *path ) ;


// dom_element /////////////////////////////////////////////////////////////////

/** Creates a new element. */
dom_element *dom_element__new( dom_document *doc,
                               char *name,
                               dom_namespace *ns ) ;

/** Destroys a element. */
void dom_element__delete( dom_element *el ) ;

/** Gets the element's name. */
const unsigned char *dom_element__name( dom_element *el ) ;

/** Gets the element's namespace (if any). */
dom_namespace *dom_element__namespace( dom_element *el ) ;

/** Associates the element with a namespace. */
void dom_element__set_namespace( dom_element *el, dom_namespace *ns );

/** Gets the element's first child (if any). */
dom_element *dom_element__first_child( dom_element *el ) ;

/** Gets the element's next sibling (if any). */
dom_element *dom_element__next_sibling( dom_element *el ) ;

/** Adds a child element to a parent element. */
void dom_element__add_child( dom_element *el, dom_element *child ) ;

/** Adds a text node to a parent element. */
void dom_element__add_text( dom_element *el, char *text ) ;

/** Gets the value of a named attribute. */
const unsigned char *dom_element__attr( dom_element *el,
                                        char *attr_name ) ;

/** Sets an attribute of an element. */
void dom_element__set_attr( dom_element *el,
                            char *attr_name,
                            char *attr_value ) ;


#endif  // XMLDOM_H

