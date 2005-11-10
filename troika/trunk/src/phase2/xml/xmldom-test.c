/**
    \file  xmldom-test.c

    \brief  Test routine for the XML toolkit interface.

    \note  This is not part of the Phase2 build.

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

#include "xmldom.h"

#include <stdio.h>  // printf


int main( int argc, char **argv )
{
    xmldom__init( );


    dom_document *doc = dom_document__new( );

    dom_element *el = dom_element__new( doc, "Root", 0 );
    dom_element__add_text( el, "Some text..." );
    dom_element__set_attr( el, "foo", "bar" );
    dom_namespace *ns = dom_namespace__new( el, "http://tempuri.org/", "tempuri" ) ;
    dom_element__set_namespace( el, ns );
    dom_document__set_root( doc, el );

    ns = dom_element__namespace( el ) ;

    dom_element *el2 = dom_element__new( doc, "ChildElement", ns );
    dom_element__add_text( el2, "Colorless green ideas sleep furiously." );
    dom_element__set_attr( el2, "programs", "data" );
    dom_element__add_child( el, el2 );

    el2 = dom_element__new( doc, "UnqualifiedElement", 0 );
    dom_element__add_text( el2, "Another node." );
    dom_element__add_child( el, el2 );

    dom_document__write_to_file( doc, "test.xml" );


    dom_document__delete( doc );

    doc = dom_document__read_from_file( "test.xml" );

    el = dom_document__root( doc ) ;
    printf( "child name = %s.\n", dom_element__name( el )) ;
    el2 = dom_element__first_child( el ) ;
    printf( "grandchild name = %s.\n", dom_element__name( el2 )) ;
    printf( "grandchild attribute \"programs\" = %s.\n", dom_element__attr( el2, "programs" )) ;

    dom_document__write_to_file( doc, "-" );


    xmldom__end( );
    return 0;
}

