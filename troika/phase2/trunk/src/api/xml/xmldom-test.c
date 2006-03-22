/**
    \file  xmldom-test.c

    \brief  Test routine for the XML toolkit interface.

    \note  This is not part of the Phase2 build.

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

#include "xmldom.h"

#include <stdio.h>  /* printf */

#define UC  ( unsigned char* )


int main( int argc, char **argv )
{
    unsigned char *s;

    xmldom__init( );

    dom_document *doc = dom_document__new( );

    dom_element *el = dom_element__new( doc, UC "Root", 0 );
    dom_element__add_text( el, UC "Some text..." );
    dom_attr *attr = dom_attr__new( el, UC "foo", UC "42", 0 );
    /*dom_element__set_attr( el, "foo", "bar" );*/
    dom_namespace *ns = dom_namespace__new( el, UC "http://tempuri.org/", UC "tempuri" ) ;
    attr = dom_attr__new( el, UC "bar", UC "137", ns );
    dom_element__set_namespace( el, ns );
    dom_document__set_root( doc, el );

    ns = dom_element__namespace( el ) ;

    dom_element *el2 = dom_element__new( doc, UC "ChildElement", ns );
    dom_element__add_text( el2, UC "Colorless green ideas sleep furiously." );
    attr = dom_attr__new(el2, UC "programs", UC "data", 0 );
    dom_element__add_child( el, el2 );

    el2 = dom_element__new( doc, UC "UnqualifiedElement", 0 );
    dom_element__add_text( el2, UC "Another node." );
    dom_element__add_child( el, el2 );

    dom_document__write_to_file( doc, "test.xml" );
    dom_document__delete( doc );

    doc = dom_document__read_from_file( "test.xml" );

    printf("----------\n");

/* Caution: memory leaks.
    el = dom_document__root( doc ) ;
    printf( "child name = %s.\n", dom_element__name( el )) ;
    printf( "child text = %s.\n", dom_element__text( el )) ;
    attr = dom_element__first_attr( el );
    printf( "child attr #1 = (%s, %s).\n",
        dom_attr__name( attr ),
        dom_attr__value( attr ));
    attr = dom_attr__next_sibling( attr );
    ns = dom_attr__namespace( attr );
    printf( "child attr #2 = (%s:%s, %s).\n",
        dom_namespace__prefix( ns ),
        dom_attr__name( attr ),
        dom_attr__value( attr ));
    el2 = dom_element__first_child( el ) ;
    printf( "grandchild name = %s.\n", dom_element__name( el2 )) ;
    attr = dom_element__attr( el2, UC "programs", 0 );
    printf( "grandchild attribute \"programs\" = %s.\n", dom_attr__value( attr )) ;
*/

    printf("----------\n");

    dom_document__write_to_file( doc, "-" );
    dom_document__delete( doc );

    printf("----------\n");


    xmldom__end( );
    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
