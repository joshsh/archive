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

#include <time.h>

#include <util/Set.h>
#include <xml/xmldom.h>
#include "../revision.h"

#include "Interpreter-impl.h"
#include "../compiler/graph.h"


typedef unsigned char uc;


typedef struct Xml_Encode_Ctx Xml_Encode_Ctx;

struct Xml_Encode_Ctx
{
    Interpreter *compiler;

    Hash_Map *serializers;

    Hash_Map *ids;

    char buffer[ENCODING__BUFFER_SIZE];
};


typedef struct Xml_Decode_Ctx Xml_Decode_Ctx;

struct Xml_Decode_Ctx
{
    Interpreter *compiler;

    Hash_Map *deserializers;

    Hash_Map *objects_by_id;

    Object *root;
};


typedef Element *( *Xml_Encoder )( void *p, Xml_Encode_Ctx *state );
typedef void *( *Xml_Decoder )( Element *el, Xml_Decode_Ctx *state );


static Element *
object__xml_encode( Object *o, Xml_Encode_Ctx *state, boolean top_level );

static Object *
object__xml_decode( Element *el, Xml_Decode_Ctx *state );


/******************************************************************************/


typedef struct Function_Wrapper Function_Wrapper;

struct Function_Wrapper
{
    Generic_f function;
};


static void *
function_wrapper__delete( Hash_Map__Entry **ppe )
{
    free( ( Function_Wrapper* ) ( *ppe )->target );
    return 0;
}


static Xml_Encoder
get_encoder( Type *t, Hash_Map *serializers )
{
    Function_Wrapper *w = hash_map__lookup( serializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Encoder ) w->function;
}


static void
set_encoder( Type *t, Xml_Encoder encode, Hash_Map *serializers )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) encode;

    hash_map__add( serializers, t, w );
}


static Xml_Decoder
get_decoder( Type *t, Hash_Map *deserializers )
{
    Function_Wrapper *w = hash_map__lookup( deserializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Decoder ) w->function;
}


static void
set_decoder( Type *t, Xml_Decoder decode, Hash_Map *deserializers )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) decode;

    hash_map__add( deserializers, t, w );
}


/* Serializers for individual types *******************************************/


static Element *
bag__xml_encode( Array *a, Xml_Encode_Ctx *state )
{
    Element *el, *child;
    int i, size;

    if ( DEBUG__SAFE && ( !a || !state ) )
        abort();

    el = element__new( 0, ( uc* ) ARRAY__XML__NAME, 0 );

    size = array__size( a );
    for ( i = 0; i < size; i++ )
    {
        child = object__xml_encode(
            ( Object* ) array__get( a, i ), state, 0 );
        element__add_child( el, child );
    }

    return el;
}


static Element *
term__xml_encode( Term *t, Xml_Encode_Ctx *state )
{
    void **sup, **head, **cur = t->head;
    Element *el, *child;

    if ( DEBUG__SAFE && ( !t || !state ) )
        abort();

    if ( ( unsigned int ) *cur == 2 )
    {
        cur++;

        el = object__xml_encode( ( Object* ) *cur, state, 0 );
    }

    else
    {
        el = element__new( 0, ( uc* ) TERM__XML__NAME, 0 );

        sup = cur + ( unsigned int ) *cur;
        cur++;
        while ( cur < sup )
        {
            head = t->head;
            t->head = cur;

            child = term__xml_encode( t, state );
            element__add_child( el, child );

            t->head = head;
            cur += ( unsigned int ) *cur;
        }
    }

    #if DEBUG__SERIAL
    printf( "[%#x] term__xml_encode(%#x, %#x)\n",
        ( int ) el, ( int ) t, ( int ) state );
    #endif

    return el;
}


static Element *
namespace__xml_encode( Namespace *ns, Xml_Encode_Ctx *state )
{
    Element *el;
    Array *keys;

    void *helper( char **name )
    {
        Object *o = namespace__lookup_simple( ns, *name );

        Element *child = object__xml_encode( o, state, 0 );
        attr__new( child, ( uc* ) "name", ( uc* ) *name, 0 );

        element__add_child( el, child );

        return 0;
    }

    if ( DEBUG__SAFE && ( !ns || !state ) )
        abort();

    el = element__new( 0, ( uc* ) NAMESPACE__XML__NAME, 0 );

    keys = namespace__keys( ns );
    array__walk( keys, ( Dist_f ) helper );
    array__delete( keys );

    return el;
}


static Element *
set__xml_encode( Set *s, Xml_Encode_Ctx *state )
{
    Element *el;

    void *helper( Object **opp )
    {
        Object *o = *opp;

        Element *child = object__xml_encode( o, state, 0 );
        element__add_child( el, child );

        return 0;
    }

    if ( DEBUG__SAFE && ( !s || !state ) )
        abort();

    el = element__new( 0, ( uc* ) SET__XML__NAME, 0 );

    set__walk( s, ( Dist_f ) helper );

    return el;
}


static Element *
apply__xml_encode( Apply *a, Xml_Encode_Ctx *state )
{
    Element *el, *child;

    if ( DEBUG__SAFE && ( !a || !state ) )
        abort();

    el = element__new( 0, ( uc* ) APPLY__XML__NAME, 0 );
    child = object__xml_encode( a->function, state, 0 );
    element__add_child( el, child );
    child = object__xml_encode( a->operand, state, 0 );
    element__add_child( el, child );

    return el;
}


/* Deserializers for individual types *****************************************/


static Array *
bag__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Array *a;
    Element *child;
    Object *o;

    if ( DEBUG__SAFE && ( !el || !state ) )
    {
        ERROR( "bag__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) element__name( el ), ARRAY__XML__NAME ) )
    {
        ERROR( "bag__xml_decode: bad element name" );
        return 0;
    }

    a = array__new( 0, 0 );

    #if DEBUG__SERIAL
    printf( "[%#x] bag__xml_decode(%#x, %#x)\n",
        ( int ) a, ( int ) el, ( int ) state );
    #endif

    child = element__first_child( el );

    while ( child )
    {
        o = object__xml_decode( child, state );
        array__enqueue( a, o );
        child = element__next_sibling( child );
    }

    return a;
}


static Term *
term__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Term *t;
    Element *child;

    if ( DEBUG__SAFE && ( !el || !state ) )
        abort();

    /* Singleton term. */
    if ( strcmp( ( char* ) element__name( el ), TERM__XML__NAME ) )
    {
        if ( DEBUG__SAFE && strcmp( ( char* ) element__name( el ), OBJECT__XML__NAME ) )
        {
            ERROR( "term__xml_decode: bad element name" );
            return 0;
        }

        t = term__new( object__xml_decode( el, state ), 0 );
    }

    /* Proper term. */
    else
    {
        child = element__first_child( el );
        t = term__xml_decode( child, state );

        while (  ( child = element__next_sibling( child ) ) )
        {
            t = term__merge_la
                ( t, term__xml_decode( child, state ) );
        }
    }

    #if DEBUG__SERIAL
    printf( "[%#x] term__xml_decode(%#x, %#x)\n",
        ( int ) t, ( int ) el, ( int ) state );
    #endif

    return t;
}


static Namespace *
namespace__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Namespace *ns;
    Element *child;
    Attr *attr;
    Object *o;
    char *text;

    if ( DEBUG__SAFE && ( !el || !state ) )
        abort();

    if ( SERIAL__CHECKS && strcmp( ( char* ) element__name( el ), NAMESPACE__XML__NAME ) )
    {
        ERROR( "namespace__xml_decode: bad element name" );
        return 0;
    }

    ns = namespace__new();

    #if DEBUG__SERIAL
    printf( "[%#x] namespace__xml_decode(%#x, %#x)\n",
        ( int ) ns, ( int ) el, ( int ) state );
    #endif

    child = element__first_child( el );
    while ( child )
    {
        attr = element__attr( child, ( uc* ) "name", 0 );

        if ( DEBUG__SAFE && !attr )
        {
            ERROR( "namespace__xml_decode: missing 'name' attribute" );
            namespace__delete( ns );
            return 0;
        }

        o = object__xml_decode( child, state );
        text = ( char* ) attr__value( attr );
        namespace__add_simple( ns, text, o );
        free( text );

        child = element__next_sibling( child );
    }

    return ns;
}


static Set *
set__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Set *s;
    Element *child;
    Object *o;

    if ( DEBUG__SAFE && ( !el || !state
    || strcmp( ( char* ) element__name( el ), SET__XML__NAME ) ) )
        abort();

    s = set__new();

    child = element__first_child( el );
    while ( child )
    {
        o = object__xml_decode( child, state );
        set__add( s, o );
        child = element__next_sibling( child );
    }

    return s;
}


static Apply *
apply__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Element *function, *operand;

    if ( DEBUG__SAFE && ( !el || !state
    || strcmp( ( char* ) element__name( el ), APPLY__XML__NAME ) ) )
        abort();

    function = element__first_child( el );
    operand = element__next_sibling( function );

    return apply__new(
        object__xml_decode( function, state ),
        object__xml_decode( operand, state ) );
}


/* Object serializer **********************************************************/


static Element *
object__xml_encode( Object *o, Xml_Encode_Ctx *state, boolean top_level )
{
    char *name;

    /* id > 0  ==>  the object is multireferenced. */
    unsigned int id = ( unsigned int ) hash_map__lookup( state->ids, o );

    Element *el;
    Xml_Encoder encode;

    /* "Short out" indirection nodes. */
    while ( object__type( o ) == indirection_type )
        o = object__value( o );

    sprintf( state->buffer, "%i", id );

    /* Element reference. */
    if ( id && !top_level )
    {
        el = element__new( 0, ( uc* ) "object", 0 );

        if ( o->type == state->compiler->combinator_t
          || o->type == state->compiler->type_t )
        {
            attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
            o->type->encode( o->value, state->buffer );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else if ( o->type == state->compiler->prim_t )
        {
            attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
            name = primitive__name( o->value );
            sprintf( state->buffer, name );
            free( name );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else
        {
            attr__new( el, ( uc* ) "ref", ( uc* ) state->buffer, 0 );
        }
    }

    /* Element data. */
    else
    {
        el = element__new( 0, ( uc* ) "object", 0 );

        /* Only multireferenced objects have ids. */
        if ( top_level )
            attr__new( el, ( uc* ) "id", ( uc* ) state->buffer, 0 );

        attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );

        encode = get_encoder( o->type, state->serializers );
/*
        encode = ( Xml_Encoder ) hash_map__lookup
            ( state->serializers, o->type );
*/

        /* Encode contents as child element. */
        if ( o->type == state->compiler->prim_t )
        {
            name = primitive__name( o->value );
            sprintf( state->buffer, name );
            free( name );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else if ( encode )
        {
            element__add_child( el, encode( o->value, state ) );
        }

        /* Encode contents as text. */
        else
        {
            o->type->encode( o->value, state->buffer );
            element__add_text( el, ( uc* ) state->buffer );
        }

    }

    return el;
}


/* Object deserializer ********************************************************/


static Object *
object__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Attr *attr;
    Object *o;
    unsigned int id;
    Type *type;
    Xml_Decoder decode;
    Element *child;
    char *text;
    Environment *env;

    if ( DEBUG__SAFE && ( !el || !state ) )
        abort();

    if ( strcmp( ( char* ) element__name( el ), OBJECT__XML__NAME ) )
    {
        ERROR( "object__xml_decode: bad element name" );
        return 0;
    }

    env = interpreter__environment( state->compiler );

    /* Full form. */
    if ( ( attr = element__attr( el, ( uc* ) "type", 0 ) ) )
    {
        text = ( char* ) attr__value( attr );

        if ( !strcmp( text, APPLY__NAME ) )
            type = apply_type;

        else
            type = environment__resolve_type( env, text )->value;

        if ( !type )
        {
            ERROR( "object__xml_decode: bad type" );
            free( text );
            return 0;
        }

        free( text );

        if ( type == state->compiler->combinator_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_combinator( env, text );
            free( text );
        }

        else if ( type == state->compiler->prim_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_primitive( env, text );
            free( text );
        }

        else if ( type == state->compiler->type_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_type( env, text );
            free( text );
        }

        else
        {
            if ( ( attr = element__attr( el, ( uc* ) "id", 0 ) ) )
            {
                text = ( char* ) attr__value( attr );
                id = ( unsigned int ) atoi( text );
                free( text );

                o = ( Object* ) hash_map__lookup( state->objects_by_id, ( void* ) id );

                if ( !o )
                {
                    /* Get a new placeholder object. */
                    o = memory_manager__object( environment__manager( env ), 0, 0, NOFLAGS );

                    hash_map__add( state->objects_by_id, ( void* ) id, o );
                }

                if ( id == 1 )
                    state->root = o;
            }

            else
            {
                /* Get a new placeholder object. */
                o = memory_manager__object( environment__manager( env ), 0, 0, NOFLAGS );
            }

            o->type = type;

            decode = get_decoder( type, state->deserializers );

            /* Decode child element. */
            if ( decode )
            {
                child = element__first_child( el );
                if ( !child )
                {
                    ERROR( "object__xml_decode: child element expected" );
                    return 0;
                }

                o->value = decode( child, state );

                if ( !o->value )
                    /* FIXME: This may cause a segfault later on. */
                    o = 0;
            }

            /* Decode element text. */
            else
            {
                text = ( char* ) element__text( el );
                o->value = type->decode( text );
                free( text );
            }
        }
    }

    /* Reference form. */
    else if ( ( attr = element__attr( el, ( uc* ) "ref", 0 ) ) )
    {
        text = ( char* ) attr__value( attr );
        id = ( unsigned int ) atoi( text );

        free( text );
        o = ( Object* ) hash_map__lookup
            ( state->objects_by_id, ( void* ) id );

        /* Placeholder object must be created. */
        if ( !o )
        {
            o = memory_manager__object( environment__manager( env ), 0, 0, NOFLAGS );
            hash_map__add( state->objects_by_id, ( void* ) id, o );
        }
    }

    else
    {
        ERROR( "object__xml_decode: missing attribute" );
        o = 0;
    }

    #if DEBUG__SERIAL
    printf( "[%#x] object__xml_decode(%#x, %#x)\n",
        ( int ) o, ( int ) el, ( int ) state );
    #endif

    return o;
}


#if TRIPLES__GLOBAL
static void
triple__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Object *subject, *predicate, *object;
    Element *subject_el, *predicate_el, *object_el;

    #if DEBUG__SERIAL
    printf( "[] triple__xml_decode(%#x, %#x)\n",
        ( int ) el, ( int ) state );
    #endif

    subject_el = element__first_child( el );
    predicate_el = element__next_sibling( subject_el );
    object_el = element__next_sibling( predicate_el );

    subject = object__xml_decode( subject_el, state );
    predicate = object__xml_decode( predicate_el, state );
    object = object__xml_decode( object_el, state );

    /* Triples have global scope. */
    object__associate( subject, predicate, object );
}
#endif


/******************************************************************************/


typedef struct Hash_Multiref_Ctx Hash_Multiref_Ctx;

struct Hash_Multiref_Ctx
{
    Hash_Map *table;
    unsigned int max;
};


static void
add_timestamp( Element *el )
{
    char *ts;
    time_t t;

    time( &t );

    /* Note: the output of localtime and asctime are evidently pointers to
       global variables, as you get a segfault if you try to free them. */
    ts = asctime( localtime( &t ) );

    *( ts + strlen( ts ) - 1 ) = '\0';
    attr__new( el, ( uc* ) "time", ( uc* ) ts, 0 );
}


static Hash_Map *
multiref_ids( Interpreter *c )
{
    Environment *env = interpreter__environment( c );
    Set *multirefs = memory_manager__get_multirefs
        ( environment__manager( env ), environment__data( env ) );

    Hash_Map *ids = hash_map__new();
    int max_id = 0;

    Object **tmp;

    void *hash_multiref( Object **opp )
    {
        /* Working namespace has already been given an id. */
        if ( !hash_map__lookup( ids, *opp ) )
        {
            max_id++;
            hash_map__add( ids, *opp, ( void* ) max_id );
        }

        return 0;
    }

    /* Force the working name space to be at top level. */
    tmp = new( Object* );
    *tmp = interpreter__working_namespace( c );
    hash_multiref( tmp );
    free( tmp );

    /* Assign all (other) multireferenced objects their ids. */
    set__walk( multirefs, ( Dist_f ) hash_multiref );
    set__delete( multirefs );

    return ids;
}


void
interpreter__serialize( Interpreter *c, char *path )
{
    Document *doc;
    Element *root;
    Xml_Encode_Ctx state;

    void *obj_helper( Hash_Map__Entry **epp )
    {
        Object *o;
        Element *el;

        void *triple_helper( Hash_Map__Entry **epp )
        {
            Hash_Map__Entry *entry = *epp;

            Element *triple = element__new( 0, ( uc* ) "triple", 0 );
            Element *subject
                = object__xml_encode( o, &state, FALSE );
            Element *predicate
                = object__xml_encode( entry->key, &state, FALSE );
            Element *object
                = object__xml_encode( entry->target, &state, FALSE );

            element__add_child( triple, subject );
            element__add_child( triple, predicate );
            element__add_child( triple, object );

            element__add_child( root, triple );

            return 0;
        }

        o = ( Object* ) ( *epp )->key;

        el = object__xml_encode( o, &state, TRUE );
        element__add_child( root, el );

        if ( TRIPLES__GLOBAL__OUT_EDGES
          && o->outbound_edges
          && hash_table__size( o->outbound_edges ) )
        {
            hash_map__walk( o->outbound_edges, ( Dist_f ) triple_helper );
        }

        return 0;
    }

    if ( DEBUG__SAFE && ( !c || !path ) )
        abort();

    #if DEBUG__SERIAL
    printf( "[] interpreter__serialize(%#x, %s)\n", ( int ) c, path );
    #endif

    xmldom__init();

    /* Root element. */
    doc = document__new();
    root = element__new( doc, ( uc* ) ENCODING__ROOT__XML__NAME, 0 );

    /* Version attribute. */
    attr__new( root, ( uc* ) "p2-version", ( uc* ) VERSION, 0 );

    #ifdef REVISION
    /* Revision attribute. */
    attr__new( root, ( uc* ) "p2-revision", ( uc* ) REVISION, 0 );
    #endif

    /* Time stamp attribute. */
    add_timestamp( root );

    document__set_root( doc, root );

    state.compiler = c;
    state.serializers = hash_map__new();
    state.ids = multiref_ids( c );

    set_encoder( apply_type, ( Xml_Encoder ) apply__xml_encode, state.serializers );
    /* Note: no encoder is needed for the indirection type, as indirection nodes are
       "invisible" to the serializer/deserializer. */
    set_encoder( c->bag_t, ( Xml_Encoder ) bag__xml_encode, state.serializers );
    set_encoder( c->ns_t, ( Xml_Encoder ) namespace__xml_encode, state.serializers );
    set_encoder( c->set_t, ( Xml_Encoder ) set__xml_encode, state.serializers );
    set_encoder( c->term_t, ( Xml_Encoder ) term__xml_encode, state.serializers );

    /* ... */

    /* Multiref objects are serialized in no particular order. */
    hash_map__walk( state.ids, ( Dist_f ) obj_helper );

    hash_map__delete( state.ids );

    hash_map__walk( state.serializers,
        ( Dist_f ) function_wrapper__delete );
    hash_map__delete( state.serializers );

    document__write_to_file( doc, path );
    document__delete( doc );

    xmldom__end();

    if ( c->save_to_path )
        free( c->save_to_path );
    c->save_to_path = STRDUP( path );
}


void
interpreter__deserialize( Interpreter *c, char *path )
{
    Xml_Decode_Ctx state = { 0, 0, 0, 0 };
    Element *el, *child;
    char *el_name;
    Document *doc;
    Environment *env;

    if ( DEBUG__SAFE && ( !c || !path ) )
        abort();

    xmldom__init();

    if ( !( doc = document__read_from_file( path ) ) )
    {
        ERROR( "interpreter__deserialize: XML read failure" );
        return;
    }

    el = document__root( doc );
    if ( !el || strcmp( ( char* ) element__name( el ), ENCODING__ROOT__XML__NAME ) )
    {
        ERROR( "interpreter__deserialize: bad or missing root element" );
        goto finish;
    }

    env = interpreter__environment( c );

    state.compiler = c;

    if ( !( state.deserializers = hash_map__new() )
      || !( state.objects_by_id = hash_map__new() ) )
        goto finish;

    set_decoder( apply_type, ( Xml_Decoder ) apply__xml_decode, state.deserializers );
    /* Note: no decoder is needed for the indirection type, as indirection nodes are
       "invisible" to the serializer/deserializer. */
    set_decoder( c->bag_t, ( Xml_Decoder ) bag__xml_decode, state.deserializers );
    set_decoder( c->ns_t, ( Xml_Decoder ) namespace__xml_decode, state.deserializers );
    set_decoder( c->set_t, ( Xml_Decoder ) set__xml_decode, state.deserializers );
    set_decoder( c->term_t, ( Xml_Decoder ) term__xml_decode, state.deserializers );

    child = element__first_child( el );

    while ( child )
    {
        el_name = ( char* ) element__name( child );

        /* Object element. */
        if ( !strcmp( el_name, OBJECT__XML__NAME ) )
        {
            object__xml_decode( child, &state );
        }

        #if TRIPLES__GLOBAL
        /* Triples element. */
        else if ( !strcmp( el_name, TRIPLES__XML__NAME ) )
        {
            triple__xml_decode( child, &state );
        }
        #endif

        else
        {
            ERROR( "interpreter__deserialize: unknown element type" );
            /*ERROR( "interpreter__deserialize: unknown element type: \"%s\"", el_name );*/
            goto finish;
        }

        child = element__next_sibling( child );
    }

    if ( !state.root || state.root->type != c->ns_t )
    {
        ERROR( "interpreter__deserialize: root namespace not found" );
        goto finish;
    }

    /* Import all objects from the document root namespace to the compiler's
       working namespace. */
    namespace__add_all( interpreter__working_namespace( c )->value, state.root->value );
/*
    dictionary__add_all(
        ( ( Namespace* ) interpreter__working_namespace( c )->value )->children,
        ( ( Namespace* ) state.root->value )->children );
*/

finish:

    document__delete( doc );

    if ( state.objects_by_id )
        hash_map__delete( state.objects_by_id );

    if ( state.deserializers )
    {
        hash_map__walk( state.deserializers,
            ( Dist_f ) function_wrapper__delete );
        hash_map__delete( state.deserializers );
    }

    xmldom__end();

    if ( c->save_to_path )
        free( c->save_to_path );
    c->save_to_path = STRDUP( path );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
