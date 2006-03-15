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

#include <serial.h>
#include <util/p2_set.h>
#include <xml/xmldom.h>


typedef unsigned char uc;


typedef struct _xml_encode_st
{
    p2_lookup_table *serializers;
    p2_lookup_table *deserializers;

    p2_lookup_table *ids;

    dom_element *parent;

} xml_encode_st;


typedef dom_element *( *xml_encoder )( p2_object *o, xml_encode_st *state );


static dom_element *p2_object__xml_encode
    ( p2_object *o, xml_encode_st *state, boolean top_level );


/* Serializers for individual types *******************************************/


static dom_element *p2_bag__xml_encode( p2_array *a, xml_encode_st *state )
{
    dom_element *el, *child;
    int i;

    #if DEBUG__SAFE
    if ( !a || !state )
    {
        ERROR( "p2_bag__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = dom_element__new( 0, ( uc* ) "list", 0 );

    for ( i = 0; i < a->size; i++ )
    {
        child = p2_object__xml_encode(
            ( p2_object* ) p2_array__get( a, i ), state, 0 );
        dom_element__add_child( el, child );
    }

    return el;
}


static dom_element *p2_term__xml_encode( p2_term *t, xml_encode_st *state )
{
    void **sup, **head, **cur = t->head;
    dom_element *el, *child;

    #if DEBUG__SAFE
    if ( !t || !state )
    {
        ERROR( "p2_term__xml_encode: null argument" );
        return 0;
    }
    #endif

    if ( ( unsigned int ) *cur == 2 )
    {
        cur++;

        el = p2_object__xml_encode( ( p2_object* ) *cur, state, 0 );
    }

    else
    {
        el = dom_element__new( 0, ( uc* ) "list", 0 );

        sup = cur + ( unsigned int ) *cur;
        cur++;
        while ( cur < sup )
        {
            head = t->head;
            t->head = cur;

            child = p2_term__xml_encode( t, state );
            dom_element__add_child( el, child );

            t->head = head;
            cur += ( unsigned int ) *cur;
        }
    }

    return el;
}


typedef struct _ns_encode_st
{
    xml_encode_st *state;
    p2_dictionary *dict;
    dom_element *parent;

} ns_encode_st;


static p2_action * ns_encode( char *name, ns_encode_st *state )
{
    p2_object *o = ( p2_object* ) p2_dictionary__lookup( state->dict, name );
printf( "---s nse 1---\n" ); fflush( stdout );

    dom_element *el = p2_object__xml_encode( o, state->state, 0 );
    dom_attr__new( el, ( uc* ) "name", ( uc* ) name, 0 );

    dom_element__add_child( state->parent, el );

    return 0;
}


static dom_element *p2_namespace__xml_encode
    ( p2_namespace *ns, xml_encode_st *state )
{
    dom_element *el;
    p2_array *keys;
    ns_encode_st nse_st;
    p2_procedure proc;
printf( "---s nsxe 1---\n" ); fflush( stdout );

    #if DEBUG__SAFE
    if ( !ns || !state )
    {
        ERROR( "p2_namespace__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = dom_element__new( 0, ( uc* ) "table", 0 );

    nse_st.state = state;
    nse_st.dict = ns->children;
    nse_st.parent = el;

    proc.execute = ( procedure ) ns_encode;
    proc.state = &nse_st;

printf( "ns = %#x\n", ( int ) ns );
printf( "ns->children->size = %i\n", ns->children->size );
    keys = p2_dictionary__keys( ns->children );
printf( "keys->size = %i\n", keys->size );
    p2_array__distribute( keys, &proc );
    p2_array__delete( keys );
printf( "---s nsxe 2---\n" ); fflush( stdout );

    return el;
}


/* Object serializer **********************************************************/


static dom_element *p2_object__xml_encode
    ( p2_object *o, xml_encode_st *state, boolean top_level )
{
    /* id > 0  ==>  the object is multireferenced. */
    unsigned int id = ( unsigned int ) p2_lookup_table__lookup( state->ids, o );

    char buffer[256];
    dom_element *el;
    xml_encoder encode;
printf( "---s oe 1---\n" ); fflush( stdout );

    sprintf( buffer, "%i", id );
printf( "---s oe 2---\n" ); fflush( stdout );

    /* Element reference. */
    if ( id && !top_level )
    {
printf( "---s oe 3a---\n" ); fflush( stdout );
        el = dom_element__new( 0, ( uc* ) "object", 0 );
        dom_attr__new( el, ( uc* ) "ref", ( uc* ) buffer, 0 );
    }

    /* Element data. */
    else
    {
printf( "---s oe 3b 1---\n" ); fflush( stdout );
        el = dom_element__new( 0, ( uc* ) "object", 0 );

        /* Only multireferenced objects have ids. */
        if ( top_level )
            dom_attr__new( el, ( uc* ) "id", ( uc* ) buffer, 0 );
printf( "---s oe 3b 2---\n" ); fflush( stdout );

        dom_attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
printf( "---s oe 3b 3---\n" ); fflush( stdout );

        encode = ( xml_encoder ) p2_lookup_table__lookup
            ( state->serializers, o->type );
printf( "---s oe 3b 4---\n" ); fflush( stdout );

        /* Encode contents as child element. */
        if ( encode )
        {
printf( "---s oe 3b 5a---\n" ); fflush( stdout );
            dom_element__add_child( el, encode( o->value, state ) );
        }

        /* Encode contents as text. */
        else
        {
printf( "---s oe 3b 5b 1 (woot)---\n" ); fflush( stdout );
printf( "o = %#x\n", ( int ) o ); fflush( stdout );
printf( "o->type = %#x\n", ( int ) o->type ); fflush( stdout );
printf( "o->type->name = %s\n", o->type->name ); fflush( stdout );
            o->type->encode( o->value, buffer );
            dom_element__add_text( el, ( uc* ) buffer );
        }
printf( "---s oe 3b 5b 2---\n" ); fflush( stdout );

    }

    return el;
}


/******************************************************************************/


typedef struct _hash_multiref_st
{
    p2_lookup_table *table;
    unsigned int max;

} hash_multiref_st;


static p2_action * hash_multiref( p2_object *o, hash_multiref_st *state )
{
    state->max++;
    p2_lookup_table__add( state->table, o, ( void* ) state->max );
    return 0;
}


static p2_action * serialize
    ( p2_lookup_table__entry *entry, xml_encode_st *state )
{
    #if DEBUG__SAFE
    if ( !entry || !state )
    {
        ERROR( "serialize: null argument" );
        return 0;
    }
    #endif

    dom_element *el = p2_object__xml_encode
        ( ( p2_object* ) entry->key, state, boolean__true );
    dom_element__add_child( state->parent, el );
    return 0;
}


void p2_compiler__serialize( p2_compiler *c, char *path )
{
    dom_document *doc;
    dom_element *el;
    p2_lookup_table *ids;
    p2_set *multirefs;
    p2_procedure proc;
    hash_multiref_st state;
    xml_encode_st encode_state;

    #if DEBUG__SAFE
    if ( !c || !path )
    {
        ERROR( "p2_compiler__serialize: null argument" );
        return;
    }
    #endif
printf( "---s s 1---\n" ); fflush( stdout );
    ids = p2_lookup_table__new();
    multirefs = p2_memory_manager__get_multirefs( c->env->manager );
printf( "---s s 2---\n" ); fflush( stdout );

    state.table = ids;
    state.max = 0;
    proc.execute = ( procedure ) hash_multiref;
    proc.state = &state;
printf( "---s s 3---\n" ); fflush( stdout );

    /* Force the working name space to be at top level. */
    p2_procedure__execute( ( &proc ), c->cur_ns_obj );
printf( "---s s 4---\n" ); fflush( stdout );

    /* Assign all (other) multireferenced objects their ids. */
    p2_set__distribute( multirefs, &proc );
printf( "---s s 5---\n" ); fflush( stdout );

    p2_set__delete( multirefs );
printf( "---s s 6---\n" ); fflush( stdout );

    doc = dom_document__new();
    el = dom_element__new( doc, ( uc* ) "data_set", 0 );
    dom_attr__new( el, ( uc* ) "p2-version", ( uc* ) VERSION, 0 );
    dom_document__set_root( doc, el );
printf( "---s s 7---\n" ); fflush( stdout );

    encode_state.serializers = p2_lookup_table__new();
    encode_state.deserializers = p2_lookup_table__new();
    encode_state.ids = ids;
    encode_state.parent = el;
printf( "---s s 8---\n" ); fflush( stdout );

    p2_lookup_table__add
        ( encode_state.serializers, c->bag_t, ( void* ) p2_bag__xml_encode );
    p2_lookup_table__add
        ( encode_state.serializers, c->env->ns_t, ( void* ) p2_namespace__xml_encode );
    p2_lookup_table__add
        ( encode_state.serializers, c->term_t, ( void* ) p2_term__xml_encode );
    /* ... */
printf( "---s s 9---\n" ); fflush( stdout );

    proc.execute = ( procedure ) serialize;
    proc.state = &encode_state;
printf( "---s s 10---\n" ); fflush( stdout );

    /* Multiref objects are serialized in no particular order. */
    p2_lookup_table__distribute( ids, &proc );
printf( "---s s 11---\n" ); fflush( stdout );

    dom_document__write_to_file( doc, path );
printf( "---s s 12---\n" ); fflush( stdout );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
