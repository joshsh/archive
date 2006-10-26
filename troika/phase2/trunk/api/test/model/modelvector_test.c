/*
    gcc modelvector_test.c -pg ../../src/.libs/libphase2.a -I../../include
*/

#include <wurfel/ModelVector.h>
#include <assert.h>


#define TRUE    1
#define FALSE   0

#define assert_size(v,s)    assert( s == modelvector__manhattan_norm( v ) )


static void
test()
{
    ModelVector *v1, *v2, *v3, *v4;
    ModelVectorDiff *diff1;
    index i1, i2, i3, i4;

    i1.intval = 123123;
    i2.intval = 34545;
    i3.intval = 678678;
    i4.intval = 49868;

    v1 = modelvector__new();
    v2 = modelvector__new();

    modelvector__set_component( v1, i1, TRUE );
    modelvector__set_component( v1, i2, TRUE );
    modelvector__set_component( v1, i3, TRUE );

    assert_size( v1, 3 );
    assert( modelvector__get_component( v1, i1 ) );
    assert( !modelvector__get_component( v1, i4 ) );

    modelvector__set_component( v2, i2, TRUE );
    modelvector__set_component( v2, i3, TRUE );
    modelvector__set_component( v2, i4, TRUE );

    assert_size( v2, 3 );
    assert( modelvector__get_component( v2, i2 ) );
    assert( !modelvector__get_component( v2, i1 ) );

    assert( modelvector__equals( v1, v1 ) );
    assert( !modelvector__equals( v1, v2 ) );

    /* Subtraction. */
    v3 = modelvector__subtract( v1, v2 );
    assert_size( v3, 1 );
    modelvector__free( v3 );

    /* Addition. */
    v3 = modelvector__add( v1, v2 );
    assert_size( v3, 4 );
    modelvector__free( v3 );

    /* Difference. */
    diff1 = modelvector__diff( v1, v2 );
    assert_size( diff1->addend, 1 );
    assert_size( diff1->subtrahend, 1 );
    v3 = modelvector__add( v1, diff1->addend );
    v4 = modelvector__subtract( v3, diff1->subtrahend );
    assert( modelvector__equals( v2, v4 ) );
    modelvector__free( v3 );
    modelvector__free( v4 );
    modelvector__free( diff1->addend );
    modelvector__free( diff1->subtrahend );
    free( diff1 );

    /* Multiplication. */
    v3 = modelvector__multiply( v1, v2 );
    assert_size( v3, 2 );
    modelvector__free( v3 );

    /* Copy. */
    v3 = modelvector__copy( v1 );
    assert( modelvector__equals( v1, v3 ) );
    modelvector__free( v3 );

    modelvector__free( v1 );
    modelvector__free( v2 );
}


int
main( int argc, char *argv[] )
{
    test();

    return 0;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
