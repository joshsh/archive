/**
    \file  xmldom-test.c

    \brief  Test routine for the XML toolkit interface.

    \note  This is not part of the Phase2 build.

    \author  Joshua Shinavier   \n
             parcour@gmail.com  \n
             +1 509 570-6990    \n */

/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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

#include <common.h>
#include <xml/xmldom.h>
#include "../settings.h"

#define UC  ( unsigned char* )


int
main( int argc, char *argv[] )
{
    /* Avoid compiler warnings. */
    argc = 0;
    argv = 0;

    Document *doc;
    Element *el, *el2;
    Namespc *ns;
    Attr *attr;

    xmldom__init();

    doc = document__new();

    el = element__new( doc, UC "Root", 0 );
    element__add_text( el, UC "Some text..." );
    attr = attr__new( el, UC "foo", UC "42", 0 );
    /*element__set_attr( el, "foo", "bar" );*/
    ns = namespc__new( el, UC "http://tempuri.org/", UC "tempuri" ) ;
    attr = attr__new( el, UC "bar", UC "137", ns );
    element__set_namespace( el, ns );
    document__set_root( doc, el );

    ns = element__namespace( el ) ;

    el2 = element__new( doc, UC "ChildElement", ns );
    element__add_text( el2, UC "Colorless green ideas sleep furiously." );
    attr = attr__new(el2, UC "programs", UC "data", 0 );
    element__add_child( el, el2 );

    el2 = element__new( doc, UC "UnqualifiedElement", 0 );
    element__add_text( el2, UC "Another node." );
    element__add_child( el, el2 );

    document__write_to_file( doc, "test.xml" );
    document__delete( doc );

    doc = document__read_from_file( "test.xml" );

    PRINT("----------\n");

/* Caution: memory leaks.
    el = document__root( doc ) ;
    PRINT( "child name = %s.\n", element__name( el )) ;
    PRINT( "child text = %s.\n", element__text( el )) ;
    attr = element__first_attr( el );
    PRINT( "child attr #1 = (%s, %s).\n",
        attr__name( attr ),
        attr__value( attr ));
    attr = attr__next_sibling( attr );
    ns = attr__namespace( attr );
    PRINT( "child attr #2 = (%s:%s, %s).\n",
        namespc__prefix( ns ),
        attr__name( attr ),
        attr__value( attr ));
    el2 = element__first_child( el ) ;
    PRINT( "grandchild name = %s.\n", element__name( el2 )) ;
    attr = element__attr( el2, UC "programs", 0 );
    PRINT( "grandchild attribute \"programs\" = %s.\n", attr__value( attr )) ;
*/

    PRINT("----------\n");

    document__write_to_file( doc, "-" );
    document__delete( doc );

    PRINT("----------\n");


    xmldom__end( );
    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
