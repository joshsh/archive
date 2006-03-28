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


static void *
function_wrapper__delete( Lookup_Table__Entry **ppe )
{
    free( ( Function_Wrapper* ) ( *ppe )->target );
    return 0;
}


static Xml_Encoder
get_encoder( Type *t, Lookup_Table *serializers )
{
    Function_Wrapper *w = lookup_table__lookup( serializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Encoder ) w->function;
}


static void
set_encoder( Type *t, Xml_Encoder encode, Lookup_Table *serializers )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) encode;

    lookup_table__add( serializers, t, w );
}


static Xml_Decoder
get_decoder( Type *t, Lookup_Table *deserializers )
{
    Function_Wrapper *w = lookup_table__lookup( deserializers, t );

    if ( !w )
        return 0;
    else
        return ( Xml_Decoder ) w->function;
}


static void
set_decoder( Type *t, Xml_Decoder decode, Lookup_Table *deserializers )
{
    Function_Wrapper *w = new( Function_Wrapper );
    w->function = ( Generic_f ) decode;

    lookup_table__add( deserializers, t, w );
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


static dom_element *
namespace__xml_encode( Namespace *ns, Xml_Encode_Ctx *state )
{
    dom_element *el;
    Array *keys;

    void *helper( char **name )
    {
        Object *o = ( Object* ) dictionary__lookup( ns->children, *name );

        dom_element *child = object__xml_encode( o, state, 0 );
        dom_attr__new( child, ( uc* ) "name", ( uc* ) *name, 0 );

        dom_element__add_child( el, child );

        return 0;
    }
printf( "---s nsxe 1---\n" ); FFLUSH;

    #if DEBUG__SAFE
    if ( !ns || !state )
    {
        ERROR( "namespace__xml_encode: null argument" );
        return 0;
    }
    #endif

    el = dom_element__new( 0, ( uc* ) NAMESPACE__XML__NAME, 0 );

printf( "ns = %#x\n", ( int ) ns );
    keys = dictionary__keys( ns->children );
printf( "array__size( keys ) = %i\n", array__size( keys ) );
    array__walk( keys, ( Dist_f ) helper );
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
printf( "o->type = %#x\n", ( int ) o->type ); FFLUSH;
printf( "o->type->name = %#x\n", ( int ) o->type->name ); FFLUSH;
printf( "o->type->name = '%s'\n", o->type->name ); FFLUSH;

        dom_attr__new( el, ( uc* ) "type", ( uc* ) o->type->name, 0 );
printf( "---s oe 3b 3---\n" ); FFLUSH;

        encode = get_encoder( o->type, state->serializers );
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

            decode = get_decoder( type, state->deserializers );

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


static void
add_timestamp( dom_element *el )
{
    char *ts;
    time_t t;

    time( &t );

    /* Note: the output of localtime and asctime are evidently pointers to
       global variables, as you get a segfault if you try to free them. */
    ts = asctime( localtime( &t ) );

    *( ts + strlen( ts ) - 1 ) = '\0';
    dom_attr__new( el, ( uc* ) "time", ( uc* ) ts, 0 );
}


static Lookup_Table *
multiref_ids( Compiler *c )
{
    Environment *env = compiler__environment( c );
    Set *multirefs  = memory_manager__get_multirefs( env->manager, env->data );

    Lookup_Table *ids = lookup_table__new();
    int max_id = 0;

    Object **tmp;

    void *hash_multiref( Object **opp )
    {
        /* Working namespace has already been given an id. */
        if ( !lookup_table__lookup( ids, *opp ) )
        {
            max_id++;
            lookup_table__add( ids, *opp, ( void* ) max_id );
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
    dom_document *doc;
    dom_element *root;
    Xml_Encode_Ctx state;
    Environment *env;

    void *obj_helper( Lookup_Table__Entry **epp )
    {
        Object *o;
        dom_element *el;

        void *triple_helper( Lookup_Table__Entry **epp )
        {
            Lookup_Table__Entry *entry = *epp;

            dom_element *triple = dom_element__new( 0, ( uc* ) "triple", 0 );
            dom_element *subject
                = object__xml_encode( o, &state, FALSE );
            dom_element *predicate
                = object__xml_encode( entry->key, &state, FALSE );
            dom_element *object
                = object__xml_encode( entry->target, &state, FALSE );

            dom_element__add_child( triple, subject );
            dom_element__add_child( triple, predicate );
            dom_element__add_child( triple, object );

            dom_element__add_child( root, triple );

            return 0;
        }

        o = ( Object* ) ( *epp )->key;

        el = object__xml_encode( o, &state, TRUE );
        dom_element__add_child( root, el );

        #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges && hash_table__size( o->outbound_edges ) )
        {
            lookup_table__walk( o->outbound_edges, ( Dist_f ) triple_helper );
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

    env = compiler__environment( c );

    xmldom__init();

printf( "---s s 6---\n" ); FFLUSH;

    /* Root element. */
    doc = dom_document__new();
    root = dom_element__new( doc, ( uc* ) ENCODING__ROOT__XML__NAME, 0 );

    /* Version attribute. */
    dom_attr__new( root, ( uc* ) "p2-version", ( uc* ) VERSION, 0 );

    /* Time stamp attribute. */
    add_timestamp( root );

    dom_document__set_root( doc, root );
printf( "---s s 7---\n" ); FFLUSH;

    state.env = env;
    state.serializers = lookup_table__new();
    state.ids = multiref_ids( c );
printf( "---s s 8---\n" ); FFLUSH;

    set_encoder( env->bag_t, ( Xml_Encoder ) bag__xml_encode, state.serializers );
    set_encoder( env->ns_t, ( Xml_Encoder ) namespace__xml_encode, state.serializers );
    set_encoder( env->term_t, ( Xml_Encoder ) term__xml_encode, state.serializers );

    /* ... */
printf( "---s s 9---\n" ); FFLUSH;

    /* Multiref objects are serialized in no particular order. */
    lookup_table__walk( state.ids, ( Dist_f ) obj_helper );
printf( "---s s 11---\n" ); FFLUSH;

    lookup_table__delete( state.ids );

    lookup_table__walk( state.serializers,
        ( Dist_f ) function_wrapper__delete );
    lookup_table__delete( state.serializers );

    dom_document__write_to_file( doc, path );
    dom_document__delete( doc );

printf( "---s s 12---\n" ); FFLUSH;
    xmldom__end();
printf( "---s s 13---\n" ); FFLUSH;
}


void
compiler__deserialize( Compiler *c, char *path )
{
    Xml_Decode_Ctx state = { 0, 0, 0, 0 };
    dom_element *el, *child;
    char *el_name;
    dom_document *doc;
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

    state.env = env;

    if ( !( state.deserializers = lookup_table__new() )
      || !( state.objects_by_id = lookup_table__new() ) )
        goto finish;

    set_decoder( env->bag_t, ( Xml_Decoder ) bag__xml_decode, state.deserializers );
    set_decoder( env->ns_t, ( Xml_Decoder ) namespace__xml_decode, state.deserializers );
    set_decoder( env->term_t, ( Xml_Decoder ) term__xml_decode, state.deserializers );

    child = dom_element__first_child( el );

    while ( child )
    {
        el_name = ( char* ) dom_element__name( child );

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
            ERROR( "compiler__deserialize: unknown element type" );
printf( " '%s'\n", el_name );
            goto finish;
        }

        child = dom_element__next_sibling( child );
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

    dom_document__delete( doc );

    if ( state.objects_by_id )
        lookup_table__delete( state.objects_by_id );

    if ( state.deserializers )
    {
        lookup_table__walk( state.deserializers,
            ( Dist_f ) function_wrapper__delete );
        lookup_table__delete( state.deserializers );
    }

    xmldom__end();
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
