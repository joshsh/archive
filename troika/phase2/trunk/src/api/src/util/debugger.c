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

#include <stdlib.h>  /* free */
#include <stdio.h>  /* printf */
#include <string.h>  /* strdup */

#include <util/Array.h>
#include <util/Bunch.h>
#include <util/Hash_Table.h>
#include <util/Set.h>
#include <util/Term.h>
#include "../settings.h"


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


void array__debug( )
{
    func_begin( "array__debug" );

    Array *array = array__new( 200, 2.0 );
    int *foo = new( int );
    array__push( array, foo );
    array__for_all( array, set__free );
    array__delete( array );

    func_end( "array__debug" );
}


void bunch__debug( )
{
    int i;

    func_begin( "bunch__debug" );

    Bunch *bunch = bunch__new( 1000 );
    for (i = 0; i < 10000; i++ )
        bunch__add( bunch, new( void* ));
    bunch__for_all( bunch, set__free );
    bunch__delete( bunch );

    bunch = bunch__new( 100 );
    for (i = 0; i < 10000; i++ )
        bunch__add( bunch, new( void* ));
    bunch__exclude_if( bunch, is_odd );
    for (i = 0; i < 10000; i++ )
        bunch__add( bunch, new( void* ));
    bunch__exclude_if( bunch, is_even );
    bunch__exclude_if( bunch, is_odd );
    bunch__delete( bunch );

    func_end( "bunch__debug" );
}


void hash_table__debug( )
{
    int i;

    func_begin( "hash_table__debug" );

    /* Test expansion *****************/

    Hash_Table *table = hash_table__new( 0, 0, 0, ADDRESS_DEFAULTS );
    hash_table__add( table, VP -1, VP 42 );
    hash_table__add( table, VP -2, VP 42 );
    for ( i = 1; i < 1000; i++ )
        hash_table__add( table, VP i, VP i );
    hash_table__delete( table );

    /* Test key cleanup ***************/

    table = hash_table__new( 0, 0, 0, ADDRESS_DEFAULTS );
    for ( i = 0; i < 1000; i++ )
        hash_table__add( table, new( void* ), VP 42 );
    hash_table__for_all_keys( table, set__free );
    hash_table__delete( table );

    /* Test target cleanup ************/

    table = hash_table__new( 0, 2, 1.5, STRING_DEFAULTS );
    hash_table__add( table, VP "1", VP strdup( "foo" ));
    hash_table__add( table, VP "2", VP strdup( "foo" ));
    hash_table__add( table, VP "3", VP strdup( "foo" ));
    hash_table__add( table, VP "4", VP strdup( "foo" ));
    hash_table__add( table, VP "5", VP strdup( "foo" ));
    hash_table__add( table, VP "6", VP strdup( "foo" ));
    hash_table__add( table, VP "7", VP strdup( "foo" ));
    hash_table__add( table, VP "8", VP strdup( "foo" ));
    hash_table__add( table, VP "9", VP strdup( "foo" ));
    hash_table__for_all_targets( table, set__free );
    hash_table__delete( table );

    /**********************************/

    func_end( "hash_table__debug" );
}


void set__debug( )
{
    int i;

    func_begin( "set__debug" );

    /* Test expansion *****************/

    Set *set = set__new( 100, 2.0, 3.0 );
    for ( i = 0; i < 500; i++ )
    {
        set__add( set, VP 1331 );
        set__add( set, VP i );
        set__add( set, VP -i );
    }
    set__delete( set );

    /* Test cleanup *******************/

    set = set__new( 1000, 0, 0 );
    for ( i = 0; i < 500; i++ )
    {
        set__add( set, new( void* ));
    }
    set__for_all( set, set__free );
    set__delete( set );

    /**********************************/

    Set *S2, *S = set__new(20, 2.0, 2.0);
    set__add(S, (void *) 5);
    set__add(S, (void *) 6);
    set__add(S, (void *) 7);
    set__add(S, (void *) 8);

    S2 = set__copy(S);

    printf("%d, %d\n", (int) set__lookup(S, (void *) 7),
    (int) set__lookup(S, (void *) 4));

    set__delete(S);
    set__delete(S2);

    /**********************************/

    func_end( "set__debug" );
}


void term__debug( )
{
    int i;

    func_begin( "term__debug" );

    /* Test merge functions ***********/

    Term *term1 = term__new( VP 42, 100 );
    Term *term2 = term__new( VP 137, 100 );
    Term *term3 = term__cat( term1, term2 );
    Term *term4 = term__copy( term3 );
    Term *term5 = term__merge_la( term3, term4 );
    term__delete( term5 );

    /* Test cleanup *******************/

    term1 = term__new( new( void* ), 0 );
    term3 = term__new( new( void* ), 100 );
    for ( i = 0; i < 42; i++ )
    {
        term2 = term__new( new( void* ), 100 );
        term1 = term__cat( term1, term2 );
        term4 = term__new( new( void* ), 100 );
        term3 = term__cat( term3, term4 );
    }
    term5 = term__merge_ra( term1, term3 );
    term__for_all( term5, set__free );
    /*term__for_all( term5, set__free );*/
    term__delete( term5 );

    func_end( "term__debug" );
}


/******************************************************************************/


int main( int argc, char **argv )
{
    func_begin( "main" );

    array__debug( );
    bunch__debug( );
    hash_table__debug( );
    set__debug( );
    term__debug( );

    func_end( "main" );

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
