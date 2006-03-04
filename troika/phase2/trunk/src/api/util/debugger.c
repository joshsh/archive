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

#include "p2_array.h"
#include "p2_bunch.h"
#include "p2_hash_table.h"
#include "p2_set.h"
#include "p2_term.h"
#include "../p2_collection.h"

#include <stdlib.h>  /* free */
#include <stdio.h>  /* printf */
#include <string.h>  /* strdup */


#define VP ( void* )


/******************************************************************************/


int stack_depth = 0;


void func_begin( char *name )
{
   int i;

   printf( "::::: " );

   for ( i = 0; i < stack_depth; i++ )
       printf( "  " );

   printf( "+ %s\n", name );

   stack_depth++;
}


void func_end( char *name )
{
   int i;
   stack_depth--;

   printf( "::::: " );

   for ( i = 0; i < stack_depth; i++ )
       printf( "  " );

   printf( "- %s\n", name );
}


/******************************************************************************/


void *is_odd( void *p )
{
    void *r = VP (( unsigned int ) p % 2 );
    if ( r )
        free( p );
    return r;
}


void *is_even( void *p )
{
    void *r = VP !(( unsigned int ) p % 2 );
    if ( r )
        free( p );
    return r;
}


void *always( void *p )
{
    free( p );
    return VP 1;
}


/******************************************************************************/


void p2_array__debug( )
{
    func_begin( "p2_array__debug" );

    p2_array *array = p2_array__new( 200, 2.0 );
    int *foo = ( int* ) malloc( sizeof( int ));
    p2_array__push( array, foo );
    p2_array__for_all( array, p2_set__free );
    p2_array__delete( array );

    func_end( "p2_array__debug" );
}


void p2_bunch__debug( )
{
    int i;

    func_begin( "p2_bunch__debug" );

    p2_bunch *bunch = p2_bunch__new( 1000 );
    for (i = 0; i < 10000; i++ )
        p2_bunch__add( bunch, malloc( sizeof( void* )));
    p2_bunch__for_all( bunch, p2_set__free );
    p2_bunch__delete( bunch );

    bunch = p2_bunch__new( 100 );
    for (i = 0; i < 10000; i++ )
        p2_bunch__add( bunch, malloc( sizeof( void* )));
    p2_bunch__exclude_if( bunch, is_odd );
    for (i = 0; i < 10000; i++ )
        p2_bunch__add( bunch, malloc( sizeof( void* )));
    p2_bunch__exclude_if( bunch, is_even );
    p2_bunch__exclude_if( bunch, is_odd );
    p2_bunch__delete( bunch );

    func_end( "p2_bunch__debug" );
}


void p2_hash_table__debug( )
{
    int i;

    func_begin( "p2_hash_table__debug" );

    /* Test expansion *****************/

    p2_hash_table *table = p2_hash_table__new( 0, 0, 0, ADDRESS_DEFAULTS );
    p2_hash_table__add( table, VP -1, VP 42 );
    p2_hash_table__add( table, VP -2, VP 42 );
    for ( i = 1; i < 1000; i++ )
        p2_hash_table__add( table, VP i, VP i );
    p2_hash_table__delete( table );

    /* Test key cleanup ***************/

    table = p2_hash_table__new( 0, 0, 0, ADDRESS_DEFAULTS );
    for ( i = 0; i < 1000; i++ )
        p2_hash_table__add( table, malloc( sizeof( void* )), VP 42 );
    p2_hash_table__for_all_keys( table, p2_set__free );
    p2_hash_table__delete( table );

    /* Test target cleanup ************/

    table = p2_hash_table__new( 0, 2, 1.5, STRING_DEFAULTS );
    p2_hash_table__add( table, VP "1", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "2", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "3", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "4", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "5", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "6", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "7", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "8", VP strdup( "foo" ));
    p2_hash_table__add( table, VP "9", VP strdup( "foo" ));
    p2_hash_table__for_all_targets( table, p2_set__free );
    p2_hash_table__delete( table );

    /**********************************/

    func_end( "p2_hash_table__debug" );
}


void p2_set__debug( )
{
    int i;

    func_begin( "p2_set__debug" );

    /* Test expansion *****************/

    p2_set *set = p2_set__new( 100, 2.0, 3.0 );
    for ( i = 0; i < 500; i++ )
    {
        p2_set__add( set, VP 1331 );
        p2_set__add( set, VP i );
        p2_set__add( set, VP -i );
    }
    p2_set__delete( set );

    /* Test cleanup *******************/

    set = p2_set__new( 1000, 0, 0 );
    for ( i = 0; i < 500; i++ )
    {
        p2_set__add( set, malloc( sizeof( void* )));
    }
    p2_set__for_all( set, p2_set__free );
    p2_set__delete( set );

    /**********************************/

    p2_set *S2, *S = p2_set__new(20, 2.0, 2.0);
    p2_set__add(S, (void *) 5);
    p2_set__add(S, (void *) 6);
    p2_set__add(S, (void *) 7);
    p2_set__add(S, (void *) 8);

    S2 = p2_set__copy(S);

    printf("%d, %d\n", (int) p2_set__lookup(S, (void *) 7),
    (int) p2_set__lookup(S, (void *) 4));

    p2_set__delete(S);
    p2_set__delete(S2);

    /**********************************/

    func_end( "p2_set__debug" );
}


void p2_term__debug( )
{
    int i;

    func_begin( "p2_term__debug" );

    /* Test merge functions ***********/

    p2_term *term1 = p2_term__new( VP 42, 100 );
    p2_term *term2 = p2_term__new( VP 137, 100 );
    p2_term *term3 = p2_term__cat( term1, term2 );
    p2_term *term4 = p2_term__copy( term3 );
    p2_term *term5 = p2_term__merge_la( term3, term4 );
    p2_term__delete( term5 );

    /* Test cleanup *******************/

    term1 = p2_term__new( malloc( sizeof( void * )), 0 );
    term3 = p2_term__new( malloc( sizeof( void * )), 100 );
    for ( i = 0; i < 42; i++ )
    {
        term2 = p2_term__new( malloc( sizeof( void * )), 100 );
        term1 = p2_term__cat( term1, term2 );
        term4 = p2_term__new( malloc( sizeof( void * )), 100 );
        term3 = p2_term__cat( term3, term4 );
    }
    term5 = p2_term__merge_ra( term1, term3 );
    p2_term__for_all( term5, p2_set__free );
    /*p2_term__for_all( term5, p2_set__free );*/
    p2_term__delete( term5 );

    func_end( "p2_term__debug" );
}


/******************************************************************************/


int main( int argc, char **argv )
{
    func_begin( "main" );

    p2_array__debug( );
    p2_bunch__debug( );
    p2_hash_table__debug( );
    p2_set__debug( );
    p2_term__debug( );

    func_end( "main" );

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
