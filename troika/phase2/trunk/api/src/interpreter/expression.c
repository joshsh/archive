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

/*
#if USE_NCURSES
#    include <ncurses.h>
#endif
*/

#include <sk.h>

#include "Interpreter-impl.h"
#include "../compiler/graph.h"


/* FIXME */
static Interpreter *compiler;


static Name *
encoding_name( Object *o )
{
                              /* FIXME */
    return interpreter__name_of( compiler, compiler->cur_ns_obj, o );
}


static void
encode__short( Object *o, char *buffer )
{
    Name *name = encoding_name( o );

    if ( !name )
        object__encode( o, buffer );

    else
        name__encode( name, buffer );
}


static void
char__encode__alt( char *c, char *buffer )
{
    sprintf( buffer, "'%c'", *c );
}


static void
double__encode__alt( double *d, char *buffer )
{
    if ( *d - ( double ) ( ( int ) *d ) )
        sprintf( buffer, "%g", *d );
    else
        sprintf( buffer, "%g.0", *d );
}


static void
string__encode__alt( char *s, char *buffer )
{
    sprintf( buffer, "\"%s\"", s );
}


static void
set__encode__alt( Set *s, char *buffer )
{
/*
    boolean first = TRUE;

*/
    void encode( Object **opp )
    {
        Object *o = *opp;

/*

        if ( !first )
        {
            sprintf( buffer, ", " );
            buffer += 2;
        }
*/
        sprintf( buffer, "    " );
        buffer += 4;

        encode__short( o, buffer );
        buffer += strlen( buffer );

        sprintf( buffer, "\n" );
        buffer++;
/*

        first = FALSE;
*/
    }

    if ( DEBUG__SAFE && ( !s || !buffer ) )
        abort();

    if ( !set__size( s ) )
        sprintf( buffer, "{}" );

    else
    {
        sprintf( buffer, "\n{\n" );
        buffer += 3;

        set__walk( s, ( Dist_f ) encode );

        sprintf( buffer, "}" );
    }
}


static void
term__encode__alt( Term *t, char *buffer )
{
    void encode( void **cur, boolean delimit )
    {
        void **lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( ( unsigned int ) *cur == 2 )
        {
            cur++;

            encode__short( *cur, buffer );
        }

        /* If the sub-term contains further sub-terms, recurse through them. */
        else
        {
            if ( delimit )
            {
                sprintf( buffer,  "(" );
                buffer++;
            }

            lim = cur + ( unsigned int ) *cur;
            cur++;
            while ( cur < lim )
            {
                encode( cur, TRUE );
                buffer += strlen( buffer );

                cur += ( unsigned int ) *cur;

                if ( cur < lim )
                {
                    sprintf( buffer, " " );
                    buffer++;
                }
            }

            if ( delimit )
            {
                sprintf( buffer, ")" );
                buffer++;
            }
        }
    }

    if ( DEBUG__SAFE && ( !t || !buffer ) )
        abort();

    sprintf( buffer, "[" );
    buffer++;

    encode( t->head, FALSE );

    sprintf( buffer, "]" );
}


static void
array__encode__alt( Array *a, char *buffer )
{
    unsigned int i;
    Object *o;

    if ( DEBUG__SAFE && ( !a || !buffer ) )
        abort();

    sprintf( buffer, "{" );
    buffer++;

    if ( array__size( a ) )
    {
        for ( i = 0; i < array__size( a ); i++ )
        {
            o = array__get( a, i );

            if ( i )
            {
                sprintf( buffer, ", " );
                buffer += 2;
            }

            encode__short( o, buffer );
            buffer += strlen( buffer );
        }
    }

    sprintf( buffer, "}" );
}


void
apply__encode__alt( Apply *a, char *buffer )
{
    Object *o;

    if ( DEBUG__SAFE && ( !a || !buffer ) )
        abort();

    o = a->function;
    encode__short( o, buffer );
    buffer += strlen( buffer );

    sprintf( buffer, " " );
    buffer++;

    o = a->operand;

    if ( DEBUG__SAFE && !FIRST_CLASS_NULL && !o )
        abort();

    /* If the operand is another Apply (and does not have a name),
       enclose it in parentheses. */
    if ( ( FIRST_CLASS_NULL && o ) && object__type( o ) == apply_type )
    {
        if ( encoding_name( o ) )
            encode__short( o, buffer );

        else
        {
            sprintf( buffer, "(" );
            buffer++;
            encode__short( o, buffer );
            buffer += strlen( buffer );
            sprintf( buffer, ")" );
        }
    }

    else
        encode__short( o, buffer );
}


/******************************************************************************/


static Object *
resolve( Ast *ast, Interpreter *c )
{
    /* Transforms an Ast into an Object, deleting the Ast along the way. */
    Object *object_for_ast( Ast* ast )
    {
        boolean ok = TRUE;

        void *helper( Ast **astpp )
        {
            *astpp = object_for_ast( *astpp );

            if ( !FIRST_CLASS_NULL && !*astpp)
            {
                ok = FALSE;
                return BREAK;
            }

            else
                return CONTINUE;
        }

        Object *o;
        Type *type;
        void *value;
        int flags = 0;

        switch ( ast->type )
        {
            case BAG_T:

                type = c->bag_t;
                value = ast->value;
                array__walk( value, ( Dist_f ) helper );
                if ( !ok )
                    array__delete( value );
                break;

            case CHAR_T:

                type = c->char_t;
                value = ast->value;
                break;

            case FLOAT_T:

                type = c->float_t;
                value = ast->value;
                break;

            case INT_T:

                type = c->int_t;
                value = ast->value;
                break;

            case NAME_T:

                /* Retrieve an existing object and exit. */
                o = interpreter__resolve( c, ( Name* ) ast->value );
                ast__delete( ast );
                return o;

            case NULL_T:

                type = 0;
                value = 0;
                break;

            case STRING_T:

                type = c->string_t;
                value = ast->value;
                break;

            case TERM_T:

                type = c->term_t;
                value = ast->value;
                term__walk( value, ( Dist_f ) helper );
                if ( !ok )
                    term__delete( value );
                break;

            default:

                abort();
        }

        free( ast );

        if ( ok )
        {
            if ( type )
            {
                /* Create and register a new object. */
                o = memory_manager__object( environment__manager( c->env ), type, value, flags );
            }

            else
                o = 0;

            return o;
        }

        else
            return 0;
    }

    Object *result = object_for_ast( ast );

    return result;
}


/******************************************************************************/


int
interpreter__evaluate_expression( Interpreter *c, Name *name, Ast *expr )
{
    int ret = 0;
    Ast *a = 0;
    Object *o;
    char print_buffer[ENCODING__BUFFER_SIZE];
    Name *oname = 0;
    Term *t;
    Array *spine;

    Encoder apply__encode, bag__encode, char__encode, double__encode, string__encode, set__encode, term__encode;

    /* See: http://www.gnu.org/prep/standards/standards.html#Conditional-Compilation */
    if ( DEBUG__SAFE && !expr )
        abort();

    /* FIXME */
    compiler = c;

    apply__encode = apply_type->encode;
    bag__encode = c->bag_t->encode;
    char__encode = c->char_t->encode;
    double__encode = c->float_t->encode;
    string__encode = c->string_t->encode;
    set__encode = c->set_t->encode;
    term__encode = c->term_t->encode;
    apply_type->encode = ( Encoder ) apply__encode__alt;
    c->bag_t->encode = ( Encoder ) array__encode__alt;
    c->char_t->encode = ( Encoder ) char__encode__alt;
    c->float_t->encode = ( Encoder ) double__encode__alt;
    c->string_t->encode = ( Encoder ) string__encode__alt;
    c->set_t->encode = ( Encoder ) set__encode__alt;
    c->term_t->encode = ( Encoder ) term__encode__alt;

    if ( name )
        a = ast__name( name );

    o = resolve( expr, c );

    /* If a term, reduce. */
    if ( o && o->type == c->term_t )
    {
        if ( SK__REDUCE_AS_GRAPH )
        {
            spine = array__new( 0, 0 );
            o = term__to_apply_tree( o->value, environment__manager( c->env ) );
            o = reduce__graph_lazy( o, spine, environment__manager( c->env ) );
            array__delete( spine );
        }

        else
        {
            t = sk_reduce( o->value,
                environment__manager( c->env ),
                c->term_t,
                c->prim_t,
                c->combinator_t,
                c->set_t );

            if ( t )
                o->value = t;

            else
                o = 0;
        }
    }

    if ( o )
    {
        if ( o->type == c->term_t )
        {
            t = o->value;
            if ( term__length( t ) == 1 )
                o = *( t->head + 1 );
        }

        if ( a )
            interpreter__define( c, name, o );

        /* Command-line output. */
        if ( !c->quiet )
        {
            oname = interpreter__name_of__full( c, c->cur_ns_obj, o );

            if ( COMPILER__SHOW_ADDRESS )
            {
                PRINT( "%p ", ( void* ) o ); FFLUSH;
            }

            PRINT( "<%s> ", o->type->name );

            if ( oname )
            {
                name__print( oname );
                PRINT( " : " );
            }

            else
                PRINT( ": " );

            object__encode( o, print_buffer );
            PRINT( print_buffer );

            PRINT( "\n" );
        }
    }

    if ( a )
        ast__delete( a );

    if ( oname )
        name__delete( oname );

    apply_type->encode = apply__encode;
    c->bag_t->encode = bag__encode;
    c->char_t->encode = char__encode;
    c->float_t->encode = double__encode;
    c->string_t->encode = string__encode;
    c->set_t->encode = set__encode;
    c->term_t->encode = term__encode;

    memory_manager__collect_if_needed( environment__manager( c->env ) );

    return ret;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
