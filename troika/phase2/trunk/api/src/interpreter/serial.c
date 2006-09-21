/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

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


#include <time.h>

#include <phase2/Set.h>
#include <phase2/xml.h>

#include "../revision.h"

#include "Interpreter-impl.h"
#include "../collection/Term-impl.h"
#include "../compiler/Apply.h"


#define ATTR_ID     "id"
#define ATTR_NAME   "name"
#define ATTR_REF    "ref"
#define ATTR_TYPE   "type"

typedef unsigned char uc;


typedef struct Xml_Encode_Ctx Xml_Encode_Ctx;

struct Xml_Encode_Ctx
{
    Interpreter *itp;

    Hash_Map *serializers;

    Hash_Map *ids;

    char buffer[ENCODING__BUFFER_SIZE];
};


typedef struct Xml_Decode_Ctx Xml_Decode_Ctx;

struct Xml_Decode_Ctx
{
    Interpreter *itp;

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


union pointer_or_int
{
    int intval;
    void *pointerval;
};


/* FIXME: data loss may occur */
static unsigned int
pointer2int( void *p )
{
    union pointer_or_int u;
    u.intval = 0;
    u.pointerval = p;
    return u.intval;
}


/* FIXME: data loss may occur */
static void *
int2pointer( unsigned int i )
{
    union pointer_or_int u;
    u.pointerval = 0;
    u.intval = i;
    return u.pointerval;
}


/******************************************************************************/


typedef struct Function_Wrapper Function_Wrapper;

struct Function_Wrapper
{
    Generic_f function;
};


static ACTION
function_wrapper__delete( Object **key, Object **target )
{
    key = 0;
    free( ( Function_Wrapper* ) *target );
    return CONTINUE;
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
    Function_Wrapper *w = NEW( Function_Wrapper );
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
    Function_Wrapper *w = NEW( Function_Wrapper );
    w->function = ( Generic_f ) decode;

    hash_map__add( deserializers, t, w );
}



static unsigned int
get_attr__id( Element *el, Xml_Decode_Ctx *state )
{
    Attr *attr;
    char *text;
    unsigned int id;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    state = 0;

    attr = element__attr( el, ( uc* ) ATTR_ID, 0 );

    if ( attr )
    {
        text = ( char* ) attr__value( attr );
        id = ( unsigned int ) atoi( text );
        free( text );
    }

    else
        id = 0;

    return id;
}


static unsigned int
get_attr__ref( Element *el, Xml_Decode_Ctx *state )
{
    Attr *attr;
    char *text;
    unsigned int id;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    state = 0;

    attr = element__attr( el, ( uc* ) ATTR_REF, 0 );

    if ( attr )
    {
        text = ( char* ) attr__value( attr );
        id = ( unsigned int ) atoi( text );
        free( text );
    }

    else
        id = 0;

    return id;
}


static Type *
get_attr__type( Element *el, Xml_Decode_Ctx *state )
{
    Attr *attr;
    char *text;
    Type *t;
    Object *o;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    attr = element__attr( el, ( uc* ) ATTR_TYPE, 0 );

    if ( attr )
    {
        text = ( char* ) attr__value( attr );

        if ( !strcmp( text, NAMEOF( APPLY ) ) )
            t = state->itp->apply_t;

        else
        {
            o = environment__resolve_type( interpreter__environment( state->itp ), text, FALSE );
            if ( !o )
                WARNING( "type '%s' not found", text );

            t = o ? o->value : 0;
        }

        free( text );
    }

    else
        t = 0;

    return t;
}


static char *
get_attr__name( Element *el, Xml_Decode_Ctx *state )
{
    Attr *attr;
    char *text;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    state = 0;

    attr = element__attr( el, ( uc* ) ATTR_NAME, 0 );

    if ( attr )
        text = ( char* ) attr__value( attr );

    else
        text = 0;

    return text;
}


/* Serializers for individual types *******************************************/


static Element *
array__xml_encode( Array *a, Xml_Encode_Ctx *state )
{
    Element *el, *child;
    int i, size;

    if ( DEBUG__SAFE && ( !a || !state ) )
        ABORT;

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
    term_cell *sup, *head, *cur = t->head;
    Element *el, *child;

    if ( DEBUG__SAFE && ( !t || !state ) )
        ABORT;

    if ( cur->intval == 2 )
    {
        cur++;

        el = object__xml_encode( ( Object* ) cur->pointerval, state, 0 );
    }

    else
    {
        el = element__new( 0, ( uc* ) TERM__XML__NAME, 0 );

        sup = cur + cur->intval;
        cur++;
        while ( cur < sup )
        {
            head = t->head;
            t->head = cur;

            child = term__xml_encode( t, state );
            element__add_child( el, child );

            t->head = head;
            cur += cur->intval;
        }
    }

    return el;
}


static Element *
namespace__xml_encode( Namespace *ns, Xml_Encode_Ctx *state )
{
    Element *el;
    Array *keys;

    ACTION helper( char **name )
    {
        Object *o = namespace__lookup_simple( ns, *name );

        Element *child = object__xml_encode( o, state, 0 );
        attr__new( child, ( uc* ) ATTR_NAME, ( uc* ) *name, 0 );

        element__add_child( el, child );

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !ns || !state ) )
        ABORT;

    el = element__new( 0, ( uc* ) NAMESPACE__XML__NAME, 0 );

    keys = namespace__keys( ns );
    array__walk( keys, ( Visitor ) helper );
    array__delete( keys );

    return el;
}


static Element *
set__xml_encode( Set *s, Xml_Encode_Ctx *state )
{
    Element *el;

    ACTION helper( Object **opp )
    {
        Object *o = *opp;

        Element *child = object__xml_encode( o, state, 0 );
        element__add_child( el, child );

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !s || !state ) )
        ABORT;

    el = element__new( 0, ( uc* ) SET__XML__NAME, 0 );

    set__walk( s, ( Visitor ) helper );

    return el;
}


static Element *
apply__xml_encode( Apply *a, Xml_Encode_Ctx *state )
{
    Element *el, *child;

    if ( DEBUG__SAFE && ( !a || !state ) )
        ABORT;

    el = element__new( 0, ( uc* ) APPLY__XML__NAME, 0 );
    child = object__xml_encode( a->function, state, 0 );
    element__add_child( el, child );
    child = object__xml_encode( a->operand, state, 0 );
    element__add_child( el, child );

    return el;
}


/* Deserializers for individual types *****************************************/


static Array *
array__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Array *a;
    Element *child;
    Object *o;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    else if ( strcmp( ( char* ) element__name( el ), ARRAY__XML__NAME ) )
    {
        ERROR( "array__xml_decode: bad element name" );
        return 0;
    }

    a = array__new( 0, 0 );

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
        ABORT;

    /* Singleton term. */
    if ( strcmp( ( char* ) element__name( el ), TERM__XML__NAME ) )
    {
        if ( DEBUG__SAFE && strcmp( ( char* ) element__name( el ), NAMEOF( OBJECT ) ) )
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

    return t;
}


static Namespace *
namespace__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Namespace *ns;
    Element *child;
    Object *o;
    char *text;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    if ( SERIAL__CHECKS && strcmp( ( char* ) element__name( el ), NAMESPACE__XML__NAME ) )
    {
        ERROR( "namespace__xml_decode: bad element name" );
        return 0;
    }

    ns = namespace__new();

    child = element__first_child( el );
    while ( child )
    {
        text = get_attr__name( child, state );

        if ( DEBUG__SAFE && !text )
        {
            ERROR( "namespace__xml_decode: missing 'name' attribute" );
            namespace__delete( ns );
            return 0;
        }

        o = object__xml_decode( child, state );
        if ( o )
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
        ABORT;

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
        ABORT;

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

    unsigned int id;

    Element *el;
    Xml_Encoder encode;

    if ( !o )
    {
        if ( FIRST_CLASS_NULL )
        {
            el = element__new( 0, ( uc* ) "object", 0 );
            attr__new( el, ( uc* ) ATTR_REF, ( uc* ) "0", 0 );
            return el;
        }

        else if ( DEBUG__SAFE )
            ABORT;
    }

    /* id > 0  ==>  the object is multireferenced. */
    id = pointer2int( hash_map__lookup( state->ids, o ) );


    /* "Short out" indirection nodes. */
    while ( object__type( o ) == state->itp->indirection_t )
        o = object__value( o );

    sprintf( state->buffer, "%i", id );

    /* Element reference. */
    if ( id && !top_level )
    {

        el = element__new( 0, ( uc* ) "object", 0 );

        if ( o->type == state->itp->combinator_t
          || o->type == state->itp->type_t )
        {
            attr__new( el, ( uc* ) ATTR_TYPE, ( uc* ) o->type->name, 0 );
            o->type->encode( o->value, state->buffer );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else if ( o->type == state->itp->prim_t )
        {
            attr__new( el, ( uc* ) ATTR_TYPE, ( uc* ) o->type->name, 0 );
            name = primitive__name( o->value );
            sprintf( state->buffer, name );
            free( name );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else
        {
            attr__new( el, ( uc* ) ATTR_REF, ( uc* ) state->buffer, 0 );
        }
    }

    /* Element data. */
    else
    {
        el = element__new( 0, ( uc* ) "object", 0 );

        /* Only multireferenced objects have ids. */
        if ( top_level )
            attr__new( el, ( uc* ) ATTR_ID, ( uc* ) state->buffer, 0 );

        attr__new( el, ( uc* ) ATTR_TYPE, ( uc* ) o->type->name, 0 );

        encode = get_encoder( o->type, state->serializers );
/*
        encode = ( Xml_Encoder ) hash_map__lookup
            ( state->serializers, o->type );
*/

        /* Encode contents as child element. */
        if ( o->type == state->itp->prim_t )
        {
            name = primitive__name( o->value );
            sprintf( state->buffer, name );
            free( name );
            element__add_text( el, ( uc* ) state->buffer );
        }

        else if ( encode )
            element__add_child( el, encode( o->value, state ) );

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
    Object *o;
    unsigned int id;
    Type *type;
    Xml_Decoder decode;
    Element *child;
    char *text;
    Environment *env;

    if ( DEBUG__SAFE && ( !el || !state ) )
        ABORT;

    if ( strcmp( ( char* ) element__name( el ), NAMEOF( OBJECT ) ) )
    {
        ERROR( "object__xml_decode: bad element name" );
        return 0;
    }

    env = interpreter__environment( state->itp );

    /* Full form. */
    type = get_attr__type( el, state );

    if ( type )
    {
        if ( type == state->itp->combinator_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_combinator( env, text );
            free( text );
        }

        else if ( type == state->itp->prim_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_primitive( env, text );
            free( text );
        }

        else if ( type == state->itp->type_t )
        {
            text = ( char* ) element__text( el );
            o = environment__resolve_type( env, text, FALSE );
            free( text );
        }

        else
        {
            id = get_attr__id( el, state );

            if ( id )
            {
                o = ( Object* ) hash_map__lookup( state->objects_by_id, int2pointer( id ) );

                if ( !o )
                {
                    /* Get a new placeholder object. */
                    o = manager__object( environment__manager( env ), 0, 0, NOFLAGS );
                    if ( !o )
                        return 0;

                    hash_map__add( state->objects_by_id, int2pointer( id ), o );
                }

                if ( id == 1 )
                    state->root = o;
            }

            else
            {
                /* Get a new placeholder object. */
                o = manager__object( environment__manager( env ), 0, 0, NOFLAGS );
                if ( !o )
                    return 0;
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
                {
/* FIXME: This could cause a segfault later on. */
                    o = 0;
                    WARNING( "decode() returned a null object" );
                }
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
    else
    {
        id = get_attr__ref( el, state );

        if ( FIRST_CLASS_NULL && !id )
            o = 0;

        else
        {
            o = ( Object* ) hash_map__lookup
                ( state->objects_by_id, int2pointer( id ) );

            /* Placeholder object must be created. */
            if ( !o )
            {
                o = manager__object( environment__manager( env ), 0, 0, NOFLAGS );
                if ( !o )
                    return 0;

                hash_map__add( state->objects_by_id, int2pointer( id ), o );
            }
        }
    }

    return o;
}


#if TRIPLES__GLOBAL
static void
triple__xml_decode( Element *el, Xml_Decode_Ctx *state )
{
    Object *subject, *predicate, *object;
    Element *subject_el, *predicate_el, *object_el;

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
    Set *multirefs = manager__get_multirefs
        ( environment__manager( env ), environment__data( env ) );

    Hash_Map *ids = hash_map__new();
    int max_id = 0;

    Object **tmp;

    ACTION hash_multiref( Object **opp )
    {
        if ( DEBUG__SAFE && !opp )
            ABORT;

        if ( FIRST_CLASS_NULL && !*opp )
            return CONTINUE;

        /* Working namespace has already been given an id. */
        if ( !hash_map__lookup( ids, *opp ) )
        {
            max_id++;
            hash_map__add( ids, *opp, int2pointer( max_id ) );
        }

        return CONTINUE;
    }

    /* Force the working namespace to be at top level. */
    tmp = NEW( Object* );
    *tmp = interpreter__working_namespace( c );
    hash_multiref( tmp );
    free( tmp );

    /* Assign all (other) multireferenced objects their ids. */
    set__walk( multirefs, ( Visitor ) hash_multiref );
    set__delete( multirefs );

    return ids;
}


static void
itp__update_save_path( Interpreter *itp, const char *path )
{
    char *tmp = itp->save_to_path;
    itp->save_to_path = STRDUP( path );
    if ( tmp )
        free( tmp );
}


void
interpreter__serialize( Interpreter *c, const char *path )
{
    Document *doc;
    Element *root;
    Xml_Encode_Ctx state;

    ACTION helper( Object **opp, void **ignored )
    {
        Object *o;
        Element *el;

        ACTION triple_helper( Object **pred, Object **obj )
        {
            Element *triple, *subject, *predicate, *object;

            if ( DEBUG__SAFE && ( !pred || !obj ) )
                ABORT;

            triple = element__new( 0, ( uc* ) "triple", 0 );
            subject = object__xml_encode( o, &state, FALSE );
            predicate = object__xml_encode( *pred, &state, FALSE );
            object = object__xml_encode( *obj, &state, FALSE );

            element__add_child( triple, subject );
            element__add_child( triple, predicate );
            element__add_child( triple, object );

            element__add_child( root, triple );

            return CONTINUE;
        }

        /* Avoid a compiler warning. */
        ignored = 0;

        if ( DEBUG__SAFE && ( !opp ) )
            ABORT;

        o = DEREF( opp );

        if ( DEBUG__SAFE && ( !o ) )
            ABORT;

        /* Create element for this object. */
        el = object__xml_encode( o, &state, TRUE );

        /* If the object is the subject of any statements, serialize them. */
        if ( TRIPLES__GLOBAL__OUT_EDGES
          && o->outbound_edges
          && hash_table__size( o->outbound_edges ) )
        {
            hash_map__walk2( o->outbound_edges, ( Visitor2 ) triple_helper );
        }

        /* Add the new element in at the top level. */
        element__add_child( root, el );

        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !c || !path || !strlen( path ) ) )
        ABORT;

    xmldom__init();

    /* Create root element. */
    doc = document__new();
    root = element__new( doc, ( uc* ) NAMEOF( DATA_SET ), 0 );

    /* Add version attribute. */
    attr__new( root, ( uc* ) "p2-version", ( uc* ) PACKAGE_VERSION, 0 );

#ifdef REVISION
    /* Add revision attribute. */
    attr__new( root, ( uc* ) "p2-revision", ( uc* ) REVISION, 0 );
#endif

    /* Time stamp attribute. */
    add_timestamp( root );

    document__set_root( doc, root );

    state.itp = c;
    state.serializers = hash_map__new();
    state.ids = multiref_ids( c );

    set_encoder( c->apply_t, ( Xml_Encoder ) apply__xml_encode, state.serializers );
    /* Note: no encoder is needed for the indirection type, as indirection nodes are
       "invisible" to the serializer/deserializer. */
    set_encoder( c->bag_t, ( Xml_Encoder ) array__xml_encode, state.serializers );
    set_encoder( c->ns_t, ( Xml_Encoder ) namespace__xml_encode, state.serializers );
    set_encoder( c->set_t, ( Xml_Encoder ) set__xml_encode, state.serializers );
    set_encoder( c->term_t, ( Xml_Encoder ) term__xml_encode, state.serializers );

    /* ... */

    /* Multiref objects are serialized in no particular order. */
    hash_map__walk2( state.ids, ( Visitor2 ) helper );

    hash_map__delete( state.ids );

    hash_map__walk2( state.serializers,
        ( Visitor2 ) function_wrapper__delete );
    hash_map__delete( state.serializers );

    document__write_to_file( doc, path );
    document__delete( doc );

    xmldom__end();

    itp__update_save_path( c, path );
}


void
interpreter__deserialize( Interpreter *c, const char *path )
{
    Xml_Decode_Ctx state = { 0, 0, 0, 0 };
    Element *el, *child;
    char *el_name;
    Document *doc;
    Environment *env;

    if ( DEBUG__SAFE && ( !c || !path || !strlen( path ) ) )
        ABORT;

    xmldom__init();

    if ( !( doc = document__read_from_file( path ) ) )
    {
        ERROR( "interpreter__deserialize: XML read failure" );
        return;
    }

    el = document__root( doc );
    if ( !el || strcmp( ( char* ) element__name( el ), NAMEOF( DATA_SET ) ) )
    {
        ERROR( "interpreter__deserialize: bad or missing root element" );
        goto finish;
    }

    env = interpreter__environment( c );

    state.itp = c;

    if ( !( state.deserializers = hash_map__new() )
      || !( state.objects_by_id = hash_map__new() ) )
        goto finish;

    set_decoder( c->apply_t, ( Xml_Decoder ) apply__xml_decode, state.deserializers );
    /* Note: no decoder is needed for the indirection type, as indirection nodes are
       "invisible" to the serializer/deserializer. */
    set_decoder( c->bag_t, ( Xml_Decoder ) array__xml_decode, state.deserializers );
    set_decoder( c->ns_t, ( Xml_Decoder ) namespace__xml_decode, state.deserializers );
    set_decoder( c->set_t, ( Xml_Decoder ) set__xml_decode, state.deserializers );
    set_decoder( c->term_t, ( Xml_Decoder ) term__xml_decode, state.deserializers );

    child = element__first_child( el );

    while ( child )
    {
        el_name = ( char* ) element__name( child );

        /* Object element. */
        if ( !strcmp( el_name, NAMEOF( OBJECT ) ) )
        {
            object__xml_decode( child, &state );
        }

#if TRIPLES__GLOBAL
        /* Triples element. */
        else if ( !strcmp( el_name, NAMEOF( TRIPLE ) ) )
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
        hash_map__walk2( state.deserializers,
            ( Visitor2 ) function_wrapper__delete );
        hash_map__delete( state.deserializers );
    }

    xmldom__end();

    itp__update_save_path( c, path );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
