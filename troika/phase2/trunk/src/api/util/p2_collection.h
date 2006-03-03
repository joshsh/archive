
typedef void *( *criterion )( void* );

typedef struct _p2_search

    p2_search__decision (*criterion)(void *);
    p2_array *results;

} p2_search;


typedef p2_object p2_collection;


p2_array *p2_collection__match( p2_collection *coll, criterion c );
void *p2_collection__first_match( p2_collection *coll, criterion c );
int p2_collection__exists( p2_collection *coll, criterion c );
int p2_collection__for_all( p2_collection *coll, criterion c );
void p2_collection__exclude_if( p2_collection *coll, criterion c );


/* Searching ******************************************************************/


typedef enum _p2_search__decision
{
    p2_search__decision__nomatch = 0,
    p2_search__decision__match

} p2_search__decision;

/**************************************/


static action add_if_matches( void *addr, void *search )
{
    if ( search->criterion( *addr ) )
        p2_array__enqueue( ( ( p2_search* ) search )->results, *addr );

    return p2_action_effect__continue;
}


p2_array *p2_collection__match( void *(*criterion)(void *), p2_collection *coll )
{
    p2_distributor d;
    p2_search search;

    search.criterion = criterion;
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




