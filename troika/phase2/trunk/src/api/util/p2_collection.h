



typedef p2_object p2_collection;



void        p2_collection__exclude_if(  p2_collection *c, criterion cr );
boolean     p2_collection__exists(      p2_collection *c, criterion cr );
void *      p2_collection__first_match( p2_collection *c, criterion cr );
boolean     p2_collection__for_all(     p2_collection *c, criterion cr );
p2_array *  p2_collection__match(       p2_collection *c, criterion cr );
void        p2_collection__sort(        p2_collection *c, comparator cmp );


/* Searching ******************************************************************/

#include "p2_array.h"

typedef struct _p2_search

    criterion match;
    p2_array *results;

} p2_search;


/**************************************/


static action add_if_matches( void *addr, void *search )
{
    if ( search->match( *addr ) )
        p2_array__enqueue( ( ( p2_search* ) search )->results, *addr );

    return p2_action_effect__continue;
}


p2_array *p2_collection__match( p2_collection *c, criterion cr )
{
    p2_distributor d;
    p2_search search;

    search.match = cr;
    search.results = p2_array__new( 0, 0 );

    d.execute = add_if_matches
    d.context = &search;

    coll->distribute( coll->value, &d );
    return ( p2_array* ) search.results;
}


void p2_collection__distribute( p2_collection *c, p2_distributor *d )
{
    int i;

    for ( int i = 0; i < c->size; i++ )
    {
        if ( p2_distributor__apply( d, ( void* ) &array[i] ) )
            break;
    }
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
