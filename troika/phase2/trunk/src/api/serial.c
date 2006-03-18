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

/*
    To do:
        [.] graceful fail when basic object (combinator / primitive / type) not found.
*/

#include <serial.h>
#include <util/p2_set.h>
#include <xml/xmldom.h>


typedef unsigned char uc;


typedef struct _xml_encode_st
{
    p2_environment *env;

    dom_element *parent;

    p2_lookup_table *serializers;

    p2_lookup_table *ids;

} xml_encode_st;


typedef struct _xml_decode_st
{
    p2_environment *env;

    p2_lookup_table *deserializers;

    p2_lookup_table *objects_by_id;

    p2_object *root;

} xml_decode_st;


typedef dom_element *( *xml_encoder )( void *p, xml_encode_st *state );
typedef void *( *xml_decoder )( dom_element *el, xml_decode_st *state );


static dom_element *p2_object__xml_encode
    ( p2_object *o, xml_encode_st *state, boolean top_level );

static p2_object *p2_object__xml_decode
    ( dom_element *el, xml_decode_st *state );


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

    el = dom_element__new( 0, ( uc* ) ARRAY__XML__NAME, 0 );

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
        el = dom_element__new( 0, ( uc* ) TERM__XML__NAME, 0 );

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
printf( "---s nsxe 1---\n" ); FFLUSH;

    #if DEBUG__SAFE
    if ( !ns || !state )
    {
        ERROR( "p2_namespace__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = dom_element__new( 0, ( uc* ) NAMESPACE__XML__NAME, 0 );

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
printf( "---s nsxe 2---\n" ); FFLUSH;

    return el;
}


/* Deserializers for individual types *****************************************/


static p2_array *p2_bag__xml_decode( dom_element *el, xml_decode_st *state )
{
    p2_array *a;
    dom_element *child;
    p2_object *o;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "p2_bag__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), ARRAY__XML__NAME ) )
    {
        ERROR( "p2_bag__xml_decode: bad element name" );
        return 0;
    }
    #endif

    a = p2_array__new( 0, 0 );

    child = dom_element__first_child( el );

    while ( child )
    {
        o = p2_object__xml_decode( child, state );
        p2_array__enqueue( a, o );
        child = dom_element__next_sibling( child );
    }

    return a;
}


static p2_term *p2_term__xml_decode( dom_element *el, xml_decode_st *state )
{
    p2_term *t;
    dom_element *child;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "p2_term__xml_decode: null argument" );
        return 0;
    }
    #endif

    /* Singleton term. */
    if ( strcmp( ( char* ) dom_element__name( el ), TERM__XML__NAME ) )
    {
        #if DEBUG__SAFE
        if ( strcmp( ( char* ) dom_element__name( el ), OBJECT__XML__NAME ) )
        {
            ERROR( "p2_term__xml_decode: bad element name" );
            return 0;
        }
        #endif

        t = p2_term__new( p2_object__xml_decode( el, state ), 0 );
    }

    /* Proper term. */
    else
    {
        child = dom_element__first_child( el );
        t = p2_term__xml_decode( child, state );

        while (  ( child = dom_element__next_sibling( child ) ) )
        {
            t = p2_term__merge_la
                ( t, p2_term__xml_decode( child, state ) );
        }
    }

printf( "returning term %#x (length = %i)\n", ( int ) t, p2_term__length( t ) );
    return t;
}


static p2_namespace *p2_namespace__xml_decode
    ( dom_element *el, xml_decode_st *state )
{
    p2_namespace *ns;
    dom_element *child;
    dom_attr *attr;
    p2_object *o;
    char *text;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "p2_namespace__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), NAMESPACE__XML__NAME ) )
    {
        ERROR( "p2_namespace__xml_decode: bad element name" );
        return 0;
    }
    #endif

    ns = p2_namespace__new();

    child = dom_element__first_child( el );
    while ( child )
    {
        attr = dom_element__attr( child, ( uc* ) "name", 0 );

        #if DEBUG__SAFE
        if ( !attr )
        {
            ERROR( "p2_namespace__xml_decode: missing 'name' attribute" );
            p2_namespace__delete( ns );
            return 0;
        }
        #endif

        o = p2_object__xml_decode( child, state );
        text = ( char* ) dom_attr__value( attr );
        p2_namespace__add_simple( ns, text, o );
        free( text );

        child = dom_element__next_sibling( child );
    }

    return ns;
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
printf( "---s oe 1---\n" ); FFLUSH;
printf( "Serializing object #%x (top_level = %i).\n", ( int ) o, top_level );
    sprintf( buffer, "%i", id );
printf( "---s oe 2---\n" ); FFLUSH;

    /* Element reference. */
    if ( id && !top_level )
    {
        el = dom_element__new( 0, ( uc* ) "object", 0 );
printf( "---s oe 3a---\n" ); FFLUSH;

        if ( o->type == state->env->combinator_t
          || o->type == state->env->prim_t
          || o->type == state->env->type_t )
        {
            dom_attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
            o->type->encode( o->value, buffer );
            dom_element__add_text( el, ( uc* ) buffer );
        }

        else
        {
            dom_attr__new( el, ( uc* ) "ref", ( uc* ) buffer, 0 );
        }
    }

    /* Element data. */
    else
    {
printf( "---s oe 3b 1---\n" ); FFLUSH;
        el = dom_element__new( 0, ( uc* ) "object", 0 );

        /* Only multireferenced objects have ids. */
        if ( top_level )
            dom_attr__new( el, ( uc* ) "id", ( uc* ) buffer, 0 );
printf( "---s oe 3b 2---\n" ); FFLUSH;

        dom_attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
printf( "---s oe 3b 3---\n" ); FFLUSH;

        encode = ( xml_encoder ) p2_lookup_table__lookup
            ( state->serializers, o->type );
printf( "---s oe 3b 4---\n" ); FFLUSH;

        /* Encode contents as child element. */
        if ( encode )
        {
printf( "---s oe 3b 5a---\n" ); FFLUSH;
            dom_element__add_child( el, encode( o->value, state ) );
        }

        /* Encode contents as text. */
        else
        {
printf( "---s oe 3b 5b 1 (woot)---\n" ); FFLUSH;
printf( "o = %#x\n", ( int ) o ); FFLUSH;
printf( "o->type = %#x\n", ( int ) o->type ); FFLUSH;
printf( "o->type->name = %s\n", o->type->name ); FFLUSH;
            o->type->encode( o->value, buffer );
            dom_element__add_text( el, ( uc* ) buffer );
        }
printf( "---s oe 3b 5b 2---\n" ); FFLUSH;

    }

    return el;
}


/* Object deserializer ********************************************************/


/*
static p2_object *reference__xml_decode
    ( dom_element *el, xml_decode_st *state )
{

}
*/


static p2_object *p2_object__xml_decode
    ( dom_element *el, xml_decode_st *state )
{
    dom_attr *attr;
    p2_object *o;
    unsigned int id;
    p2_type *type;
    xml_decoder decode;
    dom_element *child;
    char *text;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "p2_object__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), OBJECT__XML__NAME ) )
    {
        ERROR( "p2_object__xml_decode: bad element name" );
        return 0;
    }
    #endif

    /* Full form. */
    if ( ( attr = dom_element__attr( el, ( uc* ) "type", 0 ) ) )
    {
        text = ( char* ) dom_attr__value( attr );
        if ( !( type = p2_environment__resolve_type
            ( state->env, text ) ) )
        {
            ERROR( "p2_object__xml_decode: bad type" );
            free( text );
            return 0;
        }
        free( text );
printf( "Deserializing object of type: %s\n", type->name );

        if ( type == state->env->combinator_t )
        {
            text = ( char* ) dom_element__text( el );
            o = p2_namespace__lookup_simple(
                ( p2_namespace* ) state->env->combinators->value,
                text );
            free( text );
        }

        else if ( type == state->env->prim_t )
        {
            text = ( char* ) dom_element__text( el );
            o = p2_namespace__lookup_simple(
                ( p2_namespace* ) state->env->primitives->value,
                text );
            free( text );
        }

        else if ( type == state->env->type_t )
        {
            text = ( char* ) dom_element__text( el );
            o = p2_namespace__lookup_simple(
                ( p2_namespace* ) state->env->types->value,
                text );
            free( text );
        }

        else
        {
printf( "This is an imported type.\n" ); FFLUSH;
            if ( ( attr = dom_element__attr( el, ( uc* ) "id", 0 ) ) )
            {
                text = ( char* ) dom_attr__value( attr );
                id = ( unsigned int ) atoi( text );
                free( text );
printf( "id = %i\n", id ); FFLUSH;

                o = ( p2_object* ) p2_lookup_table__lookup( state->objects_by_id, ( void* ) id );

                if ( !o )
                {
                    o = p2_object__new( 0, 0, 0 );

                    /* Register the new object. */
                    p2_memory_manager__add( state->env->manager, o );

                    p2_lookup_table__add( state->objects_by_id, ( void* ) id, o );
                }

                if ( id == 1 )
                    state->root = o;
            }

            else
            {
                o = p2_object__new( 0, 0, 0 );
                /* Register the new object. */
                p2_memory_manager__add( state->env->manager, o );
            }

            o->type = type;

            decode = ( xml_decoder ) p2_lookup_table__lookup
                ( state->deserializers, type );
printf( "decode = %#x\n", ( int ) decode );

            /* Decode child element. */
            if ( decode )
            {
                child = dom_element__first_child( el );
                if ( !child )
                {
                    ERROR( "p2_object__xml_decode: child element expected" );
                    return 0;
                }

                o->flags = o->flags | OBJECT__IS_OBJ_COLL;

                o->value = decode( child, state );

                if ( !o->value )
                    /* !!! This may cause a segfault later on. */
                    o = 0;
            }

            /* Decode element text. */
            else
            {
printf( "type = '%s' (%#x)\n", type->name, ( int ) type );
printf( "type->decode = %#x\n", ( int ) type->decode );
                text = ( char* ) dom_element__text( el );
printf( "dom_element__text( el ) = \"%s\"\n", text );
                o->value = type->decode( text );
                free( text );
if ( !strcmp( type->name, "cstring" ) )
printf( "cstring value = \"%s\" (%#x)\n", ( char* ) o->value, ( int ) o->value );
/*else if ( !strcmp( type->name, "int" ) )
printf( "int value = \"%i\" (%#x)\n", *( ( int* ) o->value ), ( int ) o->value );*/
            }
        }
    }

    /* Reference form. */
    else if ( ( attr = dom_element__attr( el, ( uc* ) "ref", 0 ) ) )
    {
        text = ( char* ) dom_attr__value( attr );
        id = ( unsigned int ) atoi( text );
printf( "Deserializing reference on id = %i.\n", id ); FFLUSH;
        free( text );
        o = ( p2_object* ) p2_lookup_table__lookup
            ( state->objects_by_id, ( void* ) id );

        /* Placeholder object must be created. */
        if ( !o )
        {
            o = p2_object__new( 0, 0, 0 );
            p2_lookup_table__add( state->objects_by_id, ( void* ) id, o );
        }
    }

    else
    {
        ERROR( "p2_object__xml_decode: missing attribute" );
        o = 0;
    }
printf( "Result is object %#x.\n", ( int ) o );

    return o;
}


#if TRIPLES__GLOBAL
static void triple__xml_decode
    ( dom_element *el, xml_decode_st *state )
{
    p2_object *subject, *predicate, *object;
    dom_element *subject_el, *predicate_el, *object_el;
printf( "Deserializing triple.\n" ); FFLUSH;

    subject_el = dom_element__first_child( el );
    predicate_el = dom_element__next_sibling( subject_el );
    object_el = dom_element__next_sibling( predicate_el );

    subject = p2_object__xml_decode( subject_el, state );
    predicate = p2_object__xml_decode( predicate_el, state );
    object = p2_object__xml_decode( object_el, state );

    /* Triples have global scope. */
    p2_object__associate( subject, predicate, object );
}
#endif


/******************************************************************************/


typedef struct _hash_multiref_st
{
    p2_lookup_table *table;
    unsigned int max;

} hash_multiref_st;


static p2_action * hash_multiref( p2_object *o, hash_multiref_st *state )
{
    /* Working namespace has already been given an id. */
    if ( !p2_lookup_table__lookup( state->table, o ) )
    {
        state->max++;
        p2_lookup_table__add( state->table, o, ( void* ) state->max );
    }
    return 0;
}


typedef struct _triple__serialize_st
{
    xml_encode_st *xe_state;

    p2_object *subject;

} triple__serialize_st;


static p2_action * triple__serialize
    ( p2_lookup_table__entry *entry, triple__serialize_st *state )
{
    dom_element *el = dom_element__new( 0, ( uc* ) "triple", 0 );
    dom_element *subject = p2_object__xml_encode
        ( state->subject, state->xe_state, boolean__false );
    dom_element *predicate = p2_object__xml_encode
        ( ( p2_object* ) entry->key, state->xe_state, boolean__false );
    dom_element *object = p2_object__xml_encode
        ( ( p2_object* ) entry->target, state->xe_state, boolean__false );

    dom_element__add_child( el, subject );
    dom_element__add_child( el, predicate );
    dom_element__add_child( el, object );

    dom_element__add_child( state->xe_state->parent, el );

    return 0;
}


static p2_action * serialize
    ( p2_lookup_table__entry *entry, xml_encode_st *state )
{
    p2_object *o;
    p2_procedure proc;
    triple__serialize_st triple_st;
    dom_element *el;

    #if DEBUG__SAFE
    if ( !entry || !state )
    {
        ERROR( "serialize: null argument" );
        return 0;
    }
    #endif

    o = ( p2_object* ) entry->key;

    el = p2_object__xml_encode( o, state, boolean__true );
    dom_element__add_child( state->parent, el );

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges && o->outbound_edges->size )
    {
        triple_st.xe_state = state;
        triple_st.subject = o;
        proc.execute = ( procedure ) triple__serialize;
        proc.state = &triple_st;

        p2_lookup_table__distribute( o->outbound_edges, &proc );
    }
    #endif

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

    xmldom__init();

printf( "---s s 1---\n" ); FFLUSH;
    ids = p2_lookup_table__new();
    multirefs = p2_memory_manager__get_multirefs
        ( c->env->manager, c->env->data );
printf( "---s s 2---\n" ); FFLUSH;

    state.table = ids;
    state.max = 0;
    proc.execute = ( procedure ) hash_multiref;
    proc.state = &state;
printf( "---s s 3---\n" ); FFLUSH;

    /* Force the working name space to be at top level. */
    p2_procedure__execute( ( &proc ), c->cur_ns_obj );
printf( "---s s 4---\n" ); FFLUSH;

    /* Assign all (other) multireferenced objects their ids. */
    p2_set__distribute( multirefs, &proc );
printf( "---s s 5---\n" ); FFLUSH;

    p2_set__delete( multirefs );
printf( "---s s 6---\n" ); FFLUSH;

    doc = dom_document__new();
    el = dom_element__new( doc, ( uc* ) ENCODING__ROOT__XML__NAME, 0 );
    dom_attr__new( el, ( uc* ) "p2-version", ( uc* ) VERSION, 0 );
    dom_document__set_root( doc, el );
printf( "---s s 7---\n" ); FFLUSH;

    encode_state.env = c->env;
    encode_state.serializers = p2_lookup_table__new();
    encode_state.ids = ids;
    encode_state.parent = el;
printf( "---s s 8---\n" ); FFLUSH;

    p2_lookup_table__add
        ( encode_state.serializers, c->bag_t, ( void* ) p2_bag__xml_encode );
    p2_lookup_table__add
        ( encode_state.serializers, c->env->ns_t, ( void* ) p2_namespace__xml_encode );
    p2_lookup_table__add
        ( encode_state.serializers, c->term_t, ( void* ) p2_term__xml_encode );
    /* ... */
printf( "---s s 9---\n" ); FFLUSH;

    proc.execute = ( procedure ) serialize;
    proc.state = &encode_state;
printf( "---s s 10---\n" ); FFLUSH;

    /* Multiref objects are serialized in no particular order. */
    p2_lookup_table__distribute( ids, &proc );
printf( "---s s 11---\n" ); FFLUSH;

    p2_lookup_table__delete( ids );
    p2_lookup_table__delete( encode_state.serializers );

    dom_document__write_to_file( doc, path );
    dom_document__delete( doc );

printf( "---s s 12---\n" ); FFLUSH;
    xmldom__end();
printf( "---s s 13---\n" ); FFLUSH;
}


void p2_compiler__deserialize( p2_compiler *c, char *path )
{
    xml_decode_st decode_state = { 0, 0, 0, 0 };
    dom_element *el, *child;
    char *el_name;
    dom_document *doc;

    xmldom__init();

    if ( !( doc = dom_document__read_from_file( path ) ) )
    {
        ERROR( "p2_compiler__deserialize: XML read failure" );
        return;
    }

    el = dom_document__root( doc );
    if ( !el || strcmp( ( char* ) dom_element__name( el ), ENCODING__ROOT__XML__NAME ) )
    {
        ERROR( "p2_compiler__deserialize: bad or missing root element" );
        goto finish;
    }

    decode_state.env = c->env;

    if ( !( decode_state.deserializers = p2_lookup_table__new() )
      || !( decode_state.objects_by_id = p2_lookup_table__new() ) )
        goto finish;

    p2_lookup_table__add
        ( decode_state.deserializers, c->bag_t, ( void* ) p2_bag__xml_decode );
    p2_lookup_table__add
        ( decode_state.deserializers, c->env->ns_t, ( void* ) p2_namespace__xml_decode );
    p2_lookup_table__add
        ( decode_state.deserializers, c->term_t, ( void* ) p2_term__xml_decode );

    child = dom_element__first_child( el );

    while ( child )
    {
        el_name = ( char* ) dom_element__name( child );

        /* Object element. */
        if ( !strcmp( el_name, OBJECT__XML__NAME ) )
        {
            p2_object__xml_decode( child, &decode_state );
        }

        #if TRIPLES__GLOBAL
        /* Triples element. */
        else if ( !strcmp( el_name, TRIPLES__XML__NAME ) )
        {
            triple__xml_decode( child, &decode_state );
        }
        #endif

        else
        {
            ERROR( "p2_compiler__deserialize: unknown element type" );
printf( " '%s'\n", el_name );
            goto finish;
        }

        child = dom_element__next_sibling( child );
    }

    if ( !decode_state.root || decode_state.root->type != c->env->ns_t )
    {
        ERROR( "p2_compiler__deserialize: root namespace not found" );
        goto finish;
    }

    /* Import all objects from the document root namespace to the compiler's
       working namespace. */
    p2_dictionary__add_all(
        ( ( p2_namespace* ) c->cur_ns_obj->value )->children,
        ( ( p2_namespace* ) decode_state.root->value )->children );

finish:

    dom_document__delete( doc );

    if ( decode_state.objects_by_id )
        p2_lookup_table__delete( decode_state.objects_by_id );

    if ( decode_state.deserializers )
        p2_lookup_table__delete( decode_state.deserializers );

    xmldom__end();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
