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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <defs.h>
#include "../xmldom.h"


/* Note: be ready for linker errors if these macros are not defined. */
#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)


static int initialized = 0 ;


void xmldom__init( )
{
    if ( !initialized )
    {
        /* This initializes the library and check potential ABI mismatches between
           the version it was compiled for and the actual shared library used. */
        LIBXML_TEST_VERSION ;

        initialized = 1 ;
    }
}


void xmldom__end( )
{
    if ( initialized )
    {
        /* Free the global variables that may have been allocated by the parser. */
        xmlCleanupParser( ) ;

        /* This is to debug memory for regression tests. */
        xmlMemoryDump( ) ;

        initialized = 0 ;
    }
}


/* dom_attr *******************************************************************/


void dom_attr__delete( dom_attr *attr )
{
    xmlFreeProp(( xmlAttr* ) attr ) ;
}


const unsigned char *dom_attr__name( dom_attr *attr )
{
    return (( xmlAttr* ) attr )->name ;
}


dom_namespace *dom_attr__namespace( dom_attr *attr )
{
    return ( dom_namespace* ) (( xmlAttr* ) attr )->ns ;
}


dom_attr *dom_attr__new( dom_element *el,
                         unsigned char *name,
                         unsigned char *value,
                         dom_namespace *ns )
{
    xmlAttr *attr ;

    if ( ns )
        attr = xmlNewNsProp(( xmlNode* ) el, ( xmlNs* ) ns, name, value ) ;
    else
        attr = xmlNewProp(( xmlNode* ) el, name, value ) ;

    return ( dom_attr* ) attr ;
}


dom_attr *dom_attr__next_sibling( dom_attr *attr )
{
    return ( dom_attr* ) (( xmlAttr* ) attr )->next ;
}


const unsigned char *dom_attr__value( dom_attr *attr )
{
    return xmlNodeGetContent(( xmlNode* ) attr ) ;
}


/* dom_document ***************************************************************/


void dom_document__delete( dom_document *doc )
{
    xmlFreeDoc( ( xmlDoc* ) doc ) ;
}


dom_document *dom_document__new( )
{
    /* Note: BAD_CAST == ( xmlChar* ) == ( unsigned char* ) */
    xmlDoc *doc = xmlNewDoc( BAD_CAST "1.0" ) ;

    return ( dom_document* ) doc ;
}


dom_document *dom_document__read_from_file( char *path )
{
    xmlDoc *doc = xmlReadFile( path, NULL, 0 ) ;
    return ( dom_document * ) doc ;
}


dom_element *dom_document__root( dom_document *doc )
{
    xmlNode *root = xmlDocGetRootElement( ( xmlDoc* ) doc ) ;
    return ( dom_element* ) root ;
}


void dom_document__set_root( dom_document *doc, dom_element *root )
{
    xmlDocSetRootElement( ( xmlDoc* ) doc, ( xmlNode* ) root ) ;
}


void dom_document__write_to_file( dom_document *doc, char *path )
{
    xmlSaveFormatFileEnc( path, ( xmlDoc* ) doc, "UTF-8", 1 ) ;
}


/* dom_element ****************************************************************/


void dom_element__add_child( dom_element *el, dom_element *child )
{
    xmlAddChild( ( xmlNode* ) el,  ( xmlNode* ) child ) ;
}


void dom_element__add_text( dom_element *el, unsigned char *text )
{
    xmlNode* child = xmlNewText( text ) ;
    xmlAddChild( ( xmlNode* ) el, child ) ;
}


dom_attr *dom_element__attr( dom_element *el,
                             unsigned char *attr_name,
                             unsigned char *namespace_uri )
{
    xmlAttr *attr;

    if ( namespace_uri )
        attr = xmlHasNsProp( ( xmlNode* ) el, attr_name, namespace_uri ) ;
    else
        attr = xmlHasProp( ( xmlNode* ) el, attr_name ) ;

    return ( dom_attr* ) attr ;
}


void dom_element__delete( dom_element *el )
{
    xmlFreeNode(( xmlNode* ) el ) ;
}


dom_attr *dom_element__first_attr( dom_element *el )
{
    return ( dom_attr* ) (( xmlNode* ) el )->properties ;
}


dom_element *dom_element__first_child( dom_element *el )
{
    xmlNode *node = (( xmlNode* ) el )->children ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( dom_element* ) node ;
}


const unsigned char *dom_element__name( dom_element *el )
{
    return (( xmlNode* ) el )->name ;
}


dom_namespace *dom_element__namespace( dom_element *el )
{
    return ( dom_namespace* ) (( xmlNode* ) el)->ns ;
}


dom_element *dom_element__new( dom_document *doc,
                               unsigned char *name,
                               dom_namespace *ns )
{
    /* Note: apparently libxml2 makes its own copy of the element name. */
    xmlNode* el = xmlNewNode( 0, name );

    /* Avoid "unused parameter" warning. */
    doc = 0;

    if ( ns )
        xmlSetNs( el, ( xmlNs* ) ns );

    return ( dom_element* ) el ;
}


dom_element *dom_element__next_sibling( dom_element *el )
{
    xmlNode *node = (( xmlNode* ) el )->next ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( dom_element* ) node ;
}


void dom_element__set_namespace( dom_element *el, dom_namespace *ns )
{
    xmlSetNs( ( xmlNode* ) el, ( xmlNs* ) ns );
}


const unsigned char *dom_element__text( dom_element *el )
{
    return xmlNodeGetContent(( xmlNode* ) el ) ;
}


/* dom_namespace **************************************************************/


void dom_namespace__delete( dom_namespace *ns )
{
    xmlFreeNs(( xmlNs* ) ns ) ;
}


const unsigned char *dom_namespace__href( dom_namespace *ns )
{
    return (( xmlNs* ) ns )->href ;
}


dom_namespace *dom_namespace__new( dom_element *el,
                                   unsigned char *href,
                                   unsigned char *prefix )
{
    return ( dom_namespace* ) xmlNewNs(( xmlNode* ) el, href, prefix ) ;
}


const unsigned char *dom_namespace__prefix( dom_namespace *ns )
{
    return (( xmlNs* ) ns )->prefix ;
}


#endif  /* #if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED) */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
