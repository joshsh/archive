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
#include <xml/xmldom.h>


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


/* Attr *******************************************************************/


void attr__delete( Attr *attr )
{
    xmlFreeProp(( xmlAttr* ) attr ) ;
}


const unsigned char *attr__name( Attr *attr )
{
    return (( xmlAttr* ) attr )->name ;
}


Namespc *attr__namespace( Attr *attr )
{
    return ( Namespc* ) (( xmlAttr* ) attr )->ns ;
}


Attr *attr__new( Element *el,
                         unsigned char *name,
                         unsigned char *value,
                         Namespc *ns )
{
    xmlAttr *attr ;

    if ( ns )
        attr = xmlNewNsProp(( xmlNode* ) el, ( xmlNs* ) ns, name, value ) ;
    else
        attr = xmlNewProp(( xmlNode* ) el, name, value ) ;

    return ( Attr* ) attr ;
}


Attr *attr__next_sibling( Attr *attr )
{
    return ( Attr* ) (( xmlAttr* ) attr )->next ;
}


const unsigned char *attr__value( Attr *attr )
{
    return xmlNodeGetContent(( xmlNode* ) attr ) ;
}


/* Document ***************************************************************/


void document__delete( Document *doc )
{
    xmlFreeDoc( ( xmlDoc* ) doc ) ;
}


Document *document__new( )
{
    /* Note: BAD_CAST == ( xmlChar* ) == ( unsigned char* ) */
    xmlDoc *doc = xmlNewDoc( BAD_CAST "1.0" ) ;

    return ( Document* ) doc ;
}


Document *document__read_from_file( char *path )
{
    xmlDoc *doc = xmlReadFile( path, NULL, 0 ) ;
    return ( Document * ) doc ;
}


Element *document__root( Document *doc )
{
    xmlNode *root = xmlDocGetRootElement( ( xmlDoc* ) doc ) ;
    return ( Element* ) root ;
}


void document__set_root( Document *doc, Element *root )
{
    xmlDocSetRootElement( ( xmlDoc* ) doc, ( xmlNode* ) root ) ;
}


void document__write_to_file( Document *doc, char *path )
{
    xmlSaveFormatFileEnc( path, ( xmlDoc* ) doc, "UTF-8", 1 ) ;
}


/* Element ****************************************************************/


void element__add_child( Element *el, Element *child )
{
    xmlAddChild( ( xmlNode* ) el,  ( xmlNode* ) child ) ;
}


void element__add_text( Element *el, unsigned char *text )
{
    xmlNode* child = xmlNewText( text ) ;
    xmlAddChild( ( xmlNode* ) el, child ) ;
}


Attr *element__attr( Element *el,
                             unsigned char *attr_name,
                             unsigned char *namespace_uri )
{
    xmlAttr *attr;

    if ( namespace_uri )
        attr = xmlHasNsProp( ( xmlNode* ) el, attr_name, namespace_uri ) ;
    else
        attr = xmlHasProp( ( xmlNode* ) el, attr_name ) ;

    return ( Attr* ) attr ;
}


void element__delete( Element *el )
{
    xmlFreeNode(( xmlNode* ) el ) ;
}


Attr *element__first_attr( Element *el )
{
    return ( Attr* ) (( xmlNode* ) el )->properties ;
}


Element *element__first_child( Element *el )
{
    xmlNode *node = (( xmlNode* ) el )->children ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( Element* ) node ;
}


const unsigned char *element__name( Element *el )
{
    return (( xmlNode* ) el )->name ;
}


Namespc *element__namespace( Element *el )
{
    return ( Namespc* ) (( xmlNode* ) el)->ns ;
}


Element *element__new( Document *doc,
                               unsigned char *name,
                               Namespc *ns )
{
    /* Note: apparently libxml2 makes its own copy of the element name. */
    xmlNode* el = xmlNewNode( 0, name );

    /* Avoid "unused parameter" warning. */
    doc = 0;

    if ( ns )
        xmlSetNs( el, ( xmlNs* ) ns );

    return ( Element* ) el ;
}


Element *element__next_sibling( Element *el )
{
    xmlNode *node = (( xmlNode* ) el )->next ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( Element* ) node ;
}


void element__set_namespace( Element *el, Namespc *ns )
{
    xmlSetNs( ( xmlNode* ) el, ( xmlNs* ) ns );
}


const unsigned char *element__text( Element *el )
{
    return xmlNodeGetContent(( xmlNode* ) el ) ;
}


/* Namespc **************************************************************/


void namespc__delete( Namespc *ns )
{
    xmlFreeNs(( xmlNs* ) ns ) ;
}


const unsigned char *namespc__href( Namespc *ns )
{
    return (( xmlNs* ) ns )->href ;
}


Namespc *namespc__new( Element *el,
                                   unsigned char *href,
                                   unsigned char *prefix )
{
    return ( Namespc* ) xmlNewNs(( xmlNode* ) el, href, prefix ) ;
}


const unsigned char *namespc__prefix( Namespc *ns )
{
    return (( xmlNs* ) ns )->prefix ;
}


#endif  /* #if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED) */

/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
