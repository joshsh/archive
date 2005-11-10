// Note: look at BAD_CAST more closely.
// Add support for namespace-qualified attributes.

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

#include "../xmldom.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string.h>  // strdup, strcmp

// Note: be ready for linker errors if these macros are not defined.
#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)


// init and end ////////////////////////////////////////////////////////////////


int initialized = 0 ;


void xmldom__init( )
{
    if ( !initialized )
    {
        // This initializes the library and check potential ABI mismatches between
        // the version it was compiled for and the actual shared library used.
        LIBXML_TEST_VERSION ;

        initialized = 1 ;
    }
}


void xmldom__end( )
{
    if ( initialized )
    {
        // Free the global variables that may have been allocated by the parser.
        xmlCleanupParser( ) ;

        // This is to debug memory for regression tests.
        xmlMemoryDump( ) ;

        initialized = 0 ;
    }
}


// dom_namespace ///////////////////////////////////////////////////////////////

dom_namespace *dom_namespace__new( dom_element *el,
                                   const unsigned char *href,
                                   const unsigned char *prefix )
{
    return ( dom_namespace* ) xmlNewNs(( xmlNode* ) el, href, prefix ) ;
}


void dom_namespace__delete( dom_namespace *ns )
{
    xmlFreeNs(( xmlNs* ) ns ) ;
}


// dom_document ////////////////////////////////////////////////////////////////


dom_document *dom_document__new( )
{
    xmlDoc *doc = xmlNewDoc( BAD_CAST "1.0" ) ;
    return ( dom_document* ) doc ;
}


void dom_document__delete( dom_document *doc )
{
    xmlFreeDoc( ( xmlDoc* ) doc ) ;
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


dom_document *dom_document__read_from_file( char *path )
{
    xmlDoc *doc = xmlReadFile( path, NULL, 0 ) ;
    return ( dom_document * ) doc ;
}


// dom_element /////////////////////////////////////////////////////////////////


dom_element *dom_element__new( dom_document *doc,
                               char *name,
                               dom_namespace *ns )
{
    // Note: apparently libxml2 makes its own copy of the element name.
    xmlNode* el = xmlNewNode( 0, BAD_CAST name ) ;

    if (ns)
        xmlSetNs( el, ( xmlNs* ) ns );

    return ( dom_element* ) el ;
}


void dom_element__delete( dom_element *el )
{
    xmlFreeNode(( xmlNode* ) el ) ;
}


const unsigned char *dom_element__name( dom_element *el )
{
    return (( xmlNode* ) el )->name ;
}


dom_namespace *dom_element__namespace( dom_element *el )
{
    return ( dom_namespace* ) (( xmlNode* ) el)->ns ;
}


void dom_element__set_namespace( dom_element *el, dom_namespace *ns )
{
    xmlSetNs( ( xmlNode* ) el, ( xmlNs* ) ns );
}


dom_element *dom_element__first_child( dom_element *el )
{
    xmlNode *node = (( xmlNode* ) el )->children ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( dom_element* ) node ;
}


dom_element *dom_element__next_sibling( dom_element *el )
{
    xmlNode *node = (( xmlNode* ) el )->next ;
    while ( node && node->type != XML_ELEMENT_NODE )
        node = node->next ;

    return ( dom_element* ) node ;
}


void dom_element__add_child( dom_element *el, dom_element *child )
{
    xmlAddChild( ( xmlNode* ) el,  ( xmlNode* ) child ) ;
}


void dom_element__add_text( dom_element *el, char *text )
{
    xmlNode* child = xmlNewText( BAD_CAST text ) ;
    xmlAddChild( ( xmlNode* ) el, child ) ;
}


const unsigned char *dom_element__attr( dom_element *el,
                                        char *attr_name )
{
    return xmlGetProp(( xmlNode* ) el, BAD_CAST attr_name );
}


void dom_element__set_attr( dom_element *el,
                            char *attr_name,
                            char *attr_value )
{
    xmlNewProp( ( xmlNode* ) el, BAD_CAST attr_name, BAD_CAST attr_value ) ;
}


////////////////////////////////////////////////////////////////////////////////


#endif  // #if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

