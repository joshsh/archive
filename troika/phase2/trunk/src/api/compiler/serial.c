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
#include <revision.h>

#include "Compiler-impl.h"


typedef unsigned char uc;


typedef struct Xml_Encode_Ctx Xml_Encode_Ctx;

struct Xml_Encode_Ctx
{
    Environment *env;

    Hash_Map *serializers;

    Hash_Map *ids;
};


typedef struct Xml_Decode_Ctx Xml_Decode_Ctx;

struct Xml_Decode_Ctx
{
    Environment *env;

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

    #if DEBUG__SAFE
    if ( !a || !state )
    {
        ERROR( "bag__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = element__new( 0, ( uc* ) ARRAY__XML__NAME, 0 );

    #if DEBUG__SERIAL
    printf( "[%#x] bag__xml_encode(%#x, %#x)\n",
        ( int ) el, ( int ) a, ( int ) state );
    #endif

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

    #if DEBUG__SAFE
    if ( !t || !state )
    {
        ERROR( "term__xml_encode: null argument" );
        return 0;
    }
    #endif

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
        Object *o = ( Object* ) dictionary__lookup( ns->children, *name );

        Element *child = object__xml_encode( o, state, 0 );
        attr__new( child, ( uc* ) "name", ( uc* ) *name, 0 );

        element__add_child( el, child );

        return 0;
    }

    #if DEBUG__SAFE
    if ( !ns || !state )
    {
        ERROR( "namespace__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = element__new( 0, ( uc* ) NAMESPACE__XML__NAME, 0 );

    #if DEBUG__SERIAL
    printf( "[%#x] namespace__xml_encode(%#x, %#x)\n",
        ( int ) el, ( int ) ns, ( int ) state );
    #endif

    keys = dictionary__keys( ns->children );
    array__walk( keys, ( Dist_f ) helper );
    array__delete( keys );

    return el;
}


/* Deserializers for individual types *****************************************/


static Array *
bag__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Array *a;
    Element *child;
    Object *o;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "bag__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) element__name( el ), ARRAY__XML__NAME ) )
    {
        ERROR( "bag__xml_decode: bad element name" );
        return 0;
    }
    #endif

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

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "term__xml_decode: null argument" );
        return 0;
    }
    #endif

    /* Singleton term. */
    if ( strcmp( ( char* ) element__name( el ), TERM__XML__NAME ) )
    {
        #if DEBUG__SAFE
        if ( strcmp( ( char* ) element__name( el ), OBJECT__XML__NAME ) )
        {
            ERROR( "term__xml_decode: bad element name" );
            return 0;
        }
        #endif

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

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "namespace__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) element__name( el ), NAMESPACE__XML__NAME ) )
    {
        ERROR( "namespace__xml_decode: bad element name" );
        return 0;
    }
    #endif

    ns = namespace__new();

    #if DEBUG__SERIAL
    printf( "[%#x] namespace__xml_decode(%#x, %#x)\n",
        ( int ) ns, ( int ) el, ( int ) state );
    #endif

    child = element__first_child( el );
    while ( child )
    {
        attr = element__attr( child, ( uc* ) "name", 0 );

        #if DEBUG__SAFE
        if ( !attr )
        {
            ERROR( "namespace__xml_decode: missing 'name' attribute" );
            namespace__delete( ns );
            return 0;
        }
        #endif

        o = object__xml_decode( child, state );
        text = ( char* ) attr__value( attr );
        namespace__add_simple( ns, text, o );
        free( text );

        child = element__next_sibling( child );
    }

    return ns;
}


/* Object serializer **********************************************************/


static Element *
object__xml_encode( Object *o, Xml_Encode_Ctx *state, boolean top_level )
{
    /* id > 0  ==>  the object is multireferenced. */
    unsigned int id = ( unsigned int ) hash_map__lookup( state->ids, o );

    char buffer[256];
    Element *el;
    Xml_Encoder encode;

    sprintf( buffer, "%i", id );

    /* Element reference. */
    if ( id && !top_level )
    {
        el = element__new( 0, ( uc* ) "object", 0 );

        if ( o->type == state->env->combinator_t
          || o->type == state->env->prim_t
          || o->type == state->env->type_t )
        {
            attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
            o->type->encode( o->value, buffer );
            element__add_text( el, ( uc* ) buffer );
        }

        else
        {
            attr__new( el, ( uc* ) "ref", ( uc* ) buffer, 0 );
        }
    }

    /* Element data. */
    else
    {
        el = element__new( 0, ( uc* ) "object", 0 );

        /* Only multireferenced objects have ids. */
        if ( top_level )
            attr__new( el, ( uc* ) "id", ( uc* ) buffer, 0 );

        attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );

        encode = get_encoder( o->type, state->serializers );
/*
        encode = ( Xml_Encoder ) hash_map__lookup
            ( state->serializers, o->type );
*/

        /* Encode contents as child element. */
        if ( encode )
        {
            element__add_child( el, encode( o->value, state ) );
        }

        /* Encode contents as text. */
        else
        {
            o->type->encode( o->value, buffer );
            element__add_text( el, ( uc* ) buffer );
        }

    }

    #if DEBUG__SERIAL
    printf( "[%#x] object__xml_encode(%#x, %#x, %i)\n",
        ( int ) el, ( int ) o, ( int ) state, top_level );
    #endif

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

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "object__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) element__name( el ), OBJECT__XML__NAME ) )
    {
        ERROR( "object__xml_decode: bad element name" );
        return 0;
    }
    #endif

    /* Full form. */
    if ( ( attr = element__attr( el, ( uc* ) "type", 0 ) ) )
    {
        text = ( char* ) attr__value( attr );
        if ( !( type = environment__resolve_type
            ( state->env, text ) ) )
        {
            ERROR( "object__xml_decode: bad type" );
            free( text );
            return 0;
        }
        free( text );

        if ( type == state->env->combinator_t )
        {
            text = ( char* ) element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->combinators->value,
                text );
            free( text );
        }

        else if ( type == state->env->prim_t )
        {
            text = ( char* ) element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->primitives->value,
                text );
            free( text );
        }

        else if ( type == state->env->type_t )
        {
            text = ( char* ) element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->types->value,
                text );
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
                    o = object__new( 0, 0, 0 );

                    /* Register the new object. */
                    memory_manager__add( state->env->manager, o );

                    hash_map__add( state->objects_by_id, ( void* ) id, o );
                }

                if ( id == 1 )
                    state->root = o;
            }

            else
            {
                o = object__new( 0, 0, 0 );
                /* Register the new object. */
                memory_manager__add( state->env->manager, o );
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
                    /* !!! This may cause a segfault later on. */
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
            o = object__new( 0, 0, 0 );
            hash_map__add( state->objects_by_id, ( void* ) id, o );
            memory_manager__add( state->env->manager, o );
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
multiref_ids( Compiler *c )
{
    Environment *env = compiler__environment( c );
    Set *multirefs  = memory_manager__get_multirefs( env->manager, env->data );

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
    *tmp = compiler__working_namespace( c );
    hash_multiref( tmp );
    free( tmp );

    /* Assign all (other) multireferenced objects their ids. */
    set__walk( multirefs, ( Dist_f ) hash_multiref );
    set__delete( multirefs );

    return ids;
}


void
compiler__serialize( Compiler *c, char *path )
{
    Document *doc;
    Element *root;
    Xml_Encode_Ctx state;
    Environment *env;

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

        #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges && hash_table__size( o->outbound_edges ) )
        {
            hash_map__walk( o->outbound_edges, ( Dist_f ) triple_helper );
        }
        #endif

        return 0;
    }

    #if DEBUG__SAFE
    if ( !c || !path )
    {
        ERROR( "compiler__serialize: null argument" );
        return;
    }
    #endif

    #if DEBUG__SERIAL
    printf( "[] compiler__serialize(%#x, %s)\n", ( int ) c, path );
    #endif

    env = compiler__environment( c );

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

    state.env = env;
    state.serializers = hash_map__new();
    state.ids = multiref_ids( c );

    set_encoder( env->bag_t, ( Xml_Encoder ) bag__xml_encode, state.serializers );
    set_encoder( env->ns_t, ( Xml_Encoder ) namespace__xml_encode, state.serializers );
    set_encoder( env->term_t, ( Xml_Encoder ) term__xml_encode, state.serializers );

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
}


void
compiler__deserialize( Compiler *c, char *path )
{
    Xml_Decode_Ctx state = { 0, 0, 0, 0 };
    Element *el, *child;
    char *el_name;
    Document *doc;
    Environment *env;

    #if DEBUG__SERIAL
    printf( "[] compiler__deserialize(%#x, %s)\n", ( int ) c, path );
    #endif

    xmldom__init();

    if ( !( doc = document__read_from_file( path ) ) )
    {
        ERROR( "compiler__deserialize: XML read failure" );
        return;
    }

    el = document__root( doc );
    if ( !el || strcmp( ( char* ) element__name( el ), ENCODING__ROOT__XML__NAME ) )
    {
        ERROR( "compiler__deserialize: bad or missing root element" );
        goto finish;
    }

    env = compiler__environment( c );

    state.env = env;

    if ( !( state.deserializers = hash_map__new() )
      || !( state.objects_by_id = hash_map__new() ) )
        goto finish;

    set_decoder( env->bag_t, ( Xml_Decoder ) bag__xml_decode, state.deserializers );
    set_decoder( env->ns_t, ( Xml_Decoder ) namespace__xml_decode, state.deserializers );
    set_decoder( env->term_t, ( Xml_Decoder ) term__xml_decode, state.deserializers );

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
            ERROR( "compiler__deserialize: unknown element type: \"%s\"", el_name );
            goto finish;
        }

        child = element__next_sibling( child );
    }

    if ( !state.root || state.root->type != env->ns_t )
    {
        ERROR( "compiler__deserialize: root namespace not found" );
        goto finish;
    }

    /* Import all objects from the document root namespace to the compiler's
       working namespace. */
    dictionary__add_all(
        ( ( Namespace* ) compiler__working_namespace( c )->value )->children,
        ( ( Namespace* ) state.root->value )->children );

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
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */