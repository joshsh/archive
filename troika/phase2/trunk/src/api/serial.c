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

#include <serial.h>
#include <util/Set.h>
#include <xml/xmldom.h>


typedef unsigned char uc;


typedef struct Xml_Encode_Ctx Xml_Encode_Ctx;

struct Xml_Encode_Ctx
{
    Environment *env;

    dom_element *parent;

    Lookup_Table *serializers;

    Lookup_Table *ids;
};


typedef struct Xml_Decode_Ctx Xml_Decode_Ctx;

struct Xml_Decode_Ctx
{
    Environment *env;

    Lookup_Table *deserializers;

    Lookup_Table *objects_by_id;

    Object *root;
};


typedef dom_element *( *Xml_Encoder )( void *p, Xml_Encode_Ctx *state );
typedef void *( *Xml_Decoder )( dom_element *el, Xml_Decode_Ctx *state );


static dom_element *
object__xml_encode( Object *o, Xml_Encode_Ctx *state, boolean top_level );

static Object *
object__xml_decode( dom_element *el, Xml_Decode_Ctx *state );


/******************************************************************************/


typedef struct Function_Wrapper Function_Wrapper;

struct Function_Wrapper
{
    Generic_f function;
};


static p2_action *
function_wrapper__delete( Lookup_Table__Entry *e, void *ignored )
{
    free( ( Function_Wrapper* ) e->target );
    return 0;
}


static Xml_Encoder
get_encoder( Type *t, Xml_Encode_Ctx *state )
{
    Function_Wrapper *w = lookup_table__lookup( state->serializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Encoder ) w->function;
}


static void
set_encoder( Type *t, Xml_Encoder encode, Xml_Encode_Ctx *state )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) encode;

    lookup_table__add( state->serializers, t, w );
}


static Xml_Decoder
get_decoder( Type *t, Xml_Decode_Ctx *state )
{
    Function_Wrapper *w = lookup_table__lookup( state->deserializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Decoder ) w->function;
}


static void
set_decoder( Type *t, Xml_Decoder decode, Xml_Decode_Ctx *state )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) decode;

    lookup_table__add( state->deserializers, t, w );
}


/* Serializers for individual types *******************************************/


static dom_element *
bag__xml_encode( Array *a, Xml_Encode_Ctx *state )
{
    dom_element *el, *child;
    int i, size;

    #if DEBUG__SAFE
    if ( !a || !state )
    {
        ERROR( "bag__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = dom_element__new( 0, ( uc* ) ARRAY__XML__NAME, 0 );

    size = array__size( a );
    for ( i = 0; i < size; i++ )
    {
        child = object__xml_encode(
            ( Object* ) array__get( a, i ), state, 0 );
        dom_element__add_child( el, child );
    }

    return el;
}


static dom_element *
term__xml_encode( Term *t, Xml_Encode_Ctx *state )
{
    void **sup, **head, **cur = t->head;
    dom_element *el, *child;

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
        el = dom_element__new( 0, ( uc* ) TERM__XML__NAME, 0 );

        sup = cur + ( unsigned int ) *cur;
        cur++;
        while ( cur < sup )
        {
            head = t->head;
            t->head = cur;

            child = term__xml_encode( t, state );
            dom_element__add_child( el, child );

            t->head = head;
            cur += ( unsigned int ) *cur;
        }
    }

    return el;
}


typedef struct Ns_Encode_Ctx Ns_Encode_Ctx;

struct Ns_Encode_Ctx
{
    Xml_Encode_Ctx *state;
    Dictionary *dict;
    dom_element *parent;
};


static p2_action *
ns_encode( char *name, Ns_Encode_Ctx *state )
{
    Object *o = ( Object* ) dictionary__lookup( state->dict, name );

    dom_element *el = object__xml_encode( o, state->state, 0 );
    dom_attr__new( el, ( uc* ) "name", ( uc* ) name, 0 );

    dom_element__add_child( state->parent, el );

    return 0;
}


static dom_element *
namespace__xml_encode( Namespace *ns, Xml_Encode_Ctx *state )
{
    dom_element *el;
    Array *keys;
    Ns_Encode_Ctx nse_st;
    p2_procedure proc;
printf( "---s nsxe 1---\n" ); FFLUSH;

    #if DEBUG__SAFE
    if ( !ns || !state )
    {
        ERROR( "namespace__xml_encode: null argument" );
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
    keys = dictionary__keys( ns->children );
printf( "array__size( keys ) = %i\n", array__size( keys ) );
    array__distribute( keys, &proc );
    array__delete( keys );
printf( "---s nsxe 2---\n" ); FFLUSH;

    return el;
}


/* Deserializers for individual types *****************************************/


static Array *
bag__xml_decode( dom_element *el, Xml_Decode_Ctx *state )
{
    Array *a;
    dom_element *child;
    Object *o;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "bag__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), ARRAY__XML__NAME ) )
    {
        ERROR( "bag__xml_decode: bad element name" );
        return 0;
    }
    #endif

    a = array__new( 0, 0 );

    child = dom_element__first_child( el );

    while ( child )
    {
        o = object__xml_decode( child, state );
        array__enqueue( a, o );
        child = dom_element__next_sibling( child );
    }

    return a;
}


static Term *
term__xml_decode( dom_element *el, Xml_Decode_Ctx *state )
{
    Term *t;
    dom_element *child;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "term__xml_decode: null argument" );
        return 0;
    }
    #endif

    /* Singleton term. */
    if ( strcmp( ( char* ) dom_element__name( el ), TERM__XML__NAME ) )
    {
        #if DEBUG__SAFE
        if ( strcmp( ( char* ) dom_element__name( el ), OBJECT__XML__NAME ) )
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
        child = dom_element__first_child( el );
        t = term__xml_decode( child, state );

        while (  ( child = dom_element__next_sibling( child ) ) )
        {
            t = term__merge_la
                ( t, term__xml_decode( child, state ) );
        }
    }

printf( "returning term %#x (length = %i)\n", ( int ) t, term__length( t ) );
    return t;
}


static Namespace *
namespace__xml_decode( dom_element *el, Xml_Decode_Ctx *state )
{
    Namespace *ns;
    dom_element *child;
    dom_attr *attr;
    Object *o;
    char *text;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "namespace__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), NAMESPACE__XML__NAME ) )
    {
        ERROR( "namespace__xml_decode: bad element name" );
        return 0;
    }
    #endif

    ns = namespace__new();

    child = dom_element__first_child( el );
    while ( child )
    {
        attr = dom_element__attr( child, ( uc* ) "name", 0 );

        #if DEBUG__SAFE
        if ( !attr )
        {
            ERROR( "namespace__xml_decode: missing 'name' attribute" );
            namespace__delete( ns );
            return 0;
        }
        #endif

        o = object__xml_decode( child, state );
        text = ( char* ) dom_attr__value( attr );
        namespace__add_simple( ns, text, o );
        free( text );

        child = dom_element__next_sibling( child );
    }

    return ns;
}


/* Object serializer **********************************************************/


static dom_element *
object__xml_encode( Object *o, Xml_Encode_Ctx *state, boolean top_level )
{
    /* id > 0  ==>  the object is multireferenced. */
    unsigned int id = ( unsigned int ) lookup_table__lookup( state->ids, o );

    char buffer[256];
    dom_element *el;
    Xml_Encoder encode;
printf( "---s oe 1---\n" ); FFLUSH;
printf( "Serializing object %#x (top_level = %i).\n", ( int ) o, top_level );
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

        encode = get_encoder( o->type, state );
/*
        encode = ( Xml_Encoder ) lookup_table__lookup
            ( state->serializers, o->type );
*/
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
static Object *reference__xml_decode
    ( dom_element *el, Xml_Decode_Ctx *state )
{

}
*/


static Object *
object__xml_decode( dom_element *el, Xml_Decode_Ctx *state )
{
    dom_attr *attr;
    Object *o;
    unsigned int id;
    Type *type;
    Xml_Decoder decode;
    dom_element *child;
    char *text;

    #if DEBUG__SAFE
    if ( !el || !state )
    {
        ERROR( "object__xml_decode: null argument" );
        return 0;
    }
    else if ( strcmp( ( char* ) dom_element__name( el ), OBJECT__XML__NAME ) )
    {
        ERROR( "object__xml_decode: bad element name" );
        return 0;
    }
    #endif

    /* Full form. */
    if ( ( attr = dom_element__attr( el, ( uc* ) "type", 0 ) ) )
    {
        text = ( char* ) dom_attr__value( attr );
        if ( !( type = environment__resolve_type
            ( state->env, text ) ) )
        {
            ERROR( "object__xml_decode: bad type" );
            free( text );
            return 0;
        }
        free( text );
printf( "Deserializing object of type: %s\n", type->name );

        if ( type == state->env->combinator_t )
        {
            text = ( char* ) dom_element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->combinators->value,
                text );
            free( text );
        }

        else if ( type == state->env->prim_t )
        {
            text = ( char* ) dom_element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->primitives->value,
                text );
            free( text );
        }

        else if ( type == state->env->type_t )
        {
            text = ( char* ) dom_element__text( el );
            o = namespace__lookup_simple(
                ( Namespace* ) state->env->types->value,
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

                o = ( Object* ) lookup_table__lookup( state->objects_by_id, ( void* ) id );

                if ( !o )
                {
                    o = object__new( 0, 0, 0 );

                    /* Register the new object. */
                    memory_manager__add( state->env->manager, o );

                    lookup_table__add( state->objects_by_id, ( void* ) id, o );
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

            decode = get_decoder( type, state );

            /*
            decode = ( Xml_Decoder ) lookup_table__lookup
                ( state->deserializers, type );
            */
printf( "decode = %#x\n", ( int ) decode );

            /* Decode child element. */
            if ( decode )
            {
                child = dom_element__first_child( el );
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
        o = ( Object* ) lookup_table__lookup
            ( state->objects_by_id, ( void* ) id );

        /* Placeholder object must be created. */
        if ( !o )
        {
            o = object__new( 0, 0, 0 );
            lookup_table__add( state->objects_by_id, ( void* ) id, o );
        }
    }

    else
    {
        ERROR( "object__xml_decode: missing attribute" );
        o = 0;
    }
printf( "Result is object %#x.\n", ( int ) o );

    return o;
}


#if TRIPLES__GLOBAL
static void
triple__xml_decode( dom_element *el, Xml_Decode_Ctx *state )
{
    Object *subject, *predicate, *object;
    dom_element *subject_el, *predicate_el, *object_el;
printf( "Deserializing triple.\n" ); FFLUSH;

    subject_el = dom_element__first_child( el );
    predicate_el = dom_element__next_sibling( subject_el );
    object_el = dom_element__next_sibling( predicate_el );

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
    Lookup_Table *table;
    unsigned int max;
};


static p2_action *
hash_multiref( Object *o, Hash_Multiref_Ctx *state )
{
    /* Working namespace has already been given an id. */
    if ( !lookup_table__lookup( state->table, o ) )
    {
        state->max++;
        lookup_table__add( state->table, o, ( void* ) state->max );
    }
    return 0;
}


typedef struct Triple_Serialize_Ctx Triple_Serialize_Ctx;

struct Triple_Serialize_Ctx
{
    Xml_Encode_Ctx *xe_state;

    Object *subject;
};


static p2_action *
triple__serialize( Lookup_Table__Entry *entry, Triple_Serialize_Ctx *state )
{
    dom_element *el = dom_element__new( 0, ( uc* ) "triple", 0 );
    dom_element *subject = object__xml_encode
        ( state->subject, state->xe_state, boolean__false );
    dom_element *predicate = object__xml_encode
        ( ( Object* ) entry->key, state->xe_state, boolean__false );
    dom_element *object = object__xml_encode
        ( ( Object* ) entry->target, state->xe_state, boolean__false );

    dom_element__add_child( el, subject );
    dom_element__add_child( el, predicate );
    dom_element__add_child( el, object );

    dom_element__add_child( state->xe_state->parent, el );

    return 0;
}


static p2_action *
serialize( Lookup_Table__Entry *entry, Xml_Encode_Ctx *state )
{
    Object *o;
    p2_procedure proc;
    Triple_Serialize_Ctx triple_st;
    dom_element *el;

    #if DEBUG__SAFE
    if ( !entry || !state )
    {
        ERROR( "serialize: null argument" );
        return 0;
    }
    #endif

    o = ( Object* ) entry->key;

    el = object__xml_encode( o, state, boolean__true );
    dom_element__add_child( state->parent, el );

    #if TRIPLES__GLOBAL__OUT_EDGES
    if ( o->outbound_edges && o->outbound_edges->size )
    {
        triple_st.xe_state = state;
        triple_st.subject = o;
        proc.execute = ( procedure ) triple__serialize;
        proc.state = &triple_st;

        lookup_table__distribute( o->outbound_edges, &proc );
    }
    #endif

    return 0;
}


static void
add_timestamp( dom_element *el )
{
    char *s;
    time_t t;

    time( &t );

    /* Note: the output of localtime and asctime are evidently pointers to
       global variables, as you get a segfault if you try to free them. */
    s = asctime( localtime( &t ) );

    *( s + strlen( s ) - 1 ) = '\0';
    dom_attr__new( el, ( uc* ) "time", ( uc* ) s, 0 );
}


void
compiler__serialize( Compiler *c, char *path )
{
    dom_document *doc;
    dom_element *el;
    Lookup_Table *ids;
    Set *multirefs;
    p2_procedure proc;
    Hash_Multiref_Ctx state;
    Xml_Encode_Ctx encode_state;
    Environment *env;

    #if DEBUG__SAFE
    if ( !c || !path )
    {
        ERROR( "compiler__serialize: null argument" );
        return;
    }
    #endif

    env = compiler__environment( c );

    xmldom__init();

printf( "---s s 1---\n" ); FFLUSH;
    ids = lookup_table__new();
    multirefs = memory_manager__get_multirefs
        ( env->manager, env->data );
printf( "---s s 2---\n" ); FFLUSH;

    state.table = ids;
    state.max = 0;
    proc.execute = ( procedure ) hash_multiref;
    proc.state = &state;
printf( "---s s 3---\n" ); FFLUSH;

    /* Force the working name space to be at top level. */
    p2_procedure__execute( ( &proc ), compiler__working_namespace( c ) );
printf( "---s s 4---\n" ); FFLUSH;

    /* Assign all (other) multireferenced objects their ids. */
    set__distribute( multirefs, &proc );
printf( "---s s 5---\n" ); FFLUSH;

    set__delete( multirefs );
printf( "---s s 6---\n" ); FFLUSH;

    /* Root element. */
    doc = dom_document__new();
    el = dom_element__new( doc, ( uc* ) ENCODING__ROOT__XML__NAME, 0 );

    /* Version attribute. */
    dom_attr__new( el, ( uc* ) "p2-version", ( uc* ) VERSION, 0 );

    /* Time stamp attribute. */
    add_timestamp( el );

    dom_document__set_root( doc, el );
printf( "---s s 7---\n" ); FFLUSH;

    encode_state.env = env;
    encode_state.serializers = lookup_table__new();
    encode_state.ids = ids;
    encode_state.parent = el;
printf( "---s s 8---\n" ); FFLUSH;

    set_encoder( env->bag_t, ( Xml_Encoder ) bag__xml_encode, &encode_state );
    set_encoder( env->ns_t, ( Xml_Encoder ) namespace__xml_encode, &encode_state );
    set_encoder( env->term_t, ( Xml_Encoder ) term__xml_encode, &encode_state );

    /* ... */
printf( "---s s 9---\n" ); FFLUSH;

    proc.execute = ( procedure ) serialize;
    proc.state = &encode_state;
printf( "---s s 10---\n" ); FFLUSH;

    /* Multiref objects are serialized in no particular order. */
    lookup_table__distribute( ids, &proc );
printf( "---s s 11---\n" ); FFLUSH;

    lookup_table__delete( ids );

    proc.execute = ( procedure ) function_wrapper__delete;
    lookup_table__distribute( encode_state.serializers, &proc );
    lookup_table__delete( encode_state.serializers );

    dom_document__write_to_file( doc, path );
    dom_document__delete( doc );

printf( "---s s 12---\n" ); FFLUSH;
    xmldom__end();
printf( "---s s 13---\n" ); FFLUSH;
}


void
compiler__deserialize( Compiler *c, char *path )
{
    Xml_Decode_Ctx decode_state = { 0, 0, 0, 0 };
    dom_element *el, *child;
    char *el_name;
    dom_document *doc;
    p2_procedure proc;
    Environment *env;

    xmldom__init();

    if ( !( doc = dom_document__read_from_file( path ) ) )
    {
        ERROR( "compiler__deserialize: XML read failure" );
        return;
    }

    el = dom_document__root( doc );
    if ( !el || strcmp( ( char* ) dom_element__name( el ), ENCODING__ROOT__XML__NAME ) )
    {
        ERROR( "compiler__deserialize: bad or missing root element" );
        goto finish;
    }

    env = compiler__environment( c );

    decode_state.env = env;

    if ( !( decode_state.deserializers = lookup_table__new() )
      || !( decode_state.objects_by_id = lookup_table__new() ) )
        goto finish;

    set_decoder( env->bag_t, ( Xml_Decoder ) bag__xml_decode, &decode_state );
    set_decoder( env->ns_t, ( Xml_Decoder ) namespace__xml_decode, &decode_state );
    set_decoder( env->term_t, ( Xml_Decoder ) term__xml_decode, &decode_state );

    child = dom_element__first_child( el );

    while ( child )
    {
        el_name = ( char* ) dom_element__name( child );

        /* Object element. */
        if ( !strcmp( el_name, OBJECT__XML__NAME ) )
        {
            object__xml_decode( child, &decode_state );
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
            ERROR( "compiler__deserialize: unknown element type" );
printf( " '%s'\n", el_name );
            goto finish;
        }

        child = dom_element__next_sibling( child );
    }

    if ( !decode_state.root || decode_state.root->type != env->ns_t )
    {
        ERROR( "compiler__deserialize: root namespace not found" );
        goto finish;
    }

    /* Import all objects from the document root namespace to the compiler's
       working namespace. */
    dictionary__add_all(
        ( ( Namespace* ) compiler__working_namespace( c )->value )->children,
        ( ( Namespace* ) decode_state.root->value )->children );

finish:

    dom_document__delete( doc );

    if ( decode_state.objects_by_id )
        lookup_table__delete( decode_state.objects_by_id );

    if ( decode_state.deserializers )
    {
        proc.execute = ( procedure ) function_wrapper__delete;
        lookup_table__distribute( decode_state.deserializers, &proc );
        lookup_table__delete( decode_state.deserializers );
    }

    xmldom__end();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
