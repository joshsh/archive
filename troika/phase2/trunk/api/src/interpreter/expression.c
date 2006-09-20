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

#include "Interpreter-impl.h"
#include "../collection/Term-impl.h"
#include "compiler/Apply.h"


/* FIXME */
static Interpreter *interpreter;


static Name *
encoding_name( Object *o )
{
                              /* FIXME */
    return interpreter__name_of( interpreter, interpreter->cur_ns_obj, o );
}


static void
encode__short( Object *o, char *buffer )
{
    interpreter__encode( interpreter, o, buffer, 0 );
/*
    Name *name = encoding_name( o );

    if ( !name )
        object__encode( o, buffer );

    else
        name__encode( name, buffer );
*/
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
    ACTION encode( Object **opp )
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
        return CONTINUE;
    }

    if ( DEBUG__SAFE && ( !s || !buffer ) )
        ABORT;

    if ( !set__size( s ) )
        sprintf( buffer, "{}" );

    else
    {
        sprintf( buffer, "\n{\n" );
        buffer += 3;

        set__walk( s, ( Visitor ) encode );

        sprintf( buffer, "}" );
    }
}


static void
term__encode__alt( Term *t, char *buffer )
{
    void encode( term_cell *cur, boolean delimit )
    {
        term_cell *lim;

        /* If the sub-term represents a leaf node, execute the procedure. */
        if ( cur->intval == 2 )
        {
            cur++;

            encode__short( cur->pointerval, buffer );
        }

        /* If the sub-term contains further sub-terms, recurse through them. */
        else
        {
            if ( delimit )
            {
                sprintf( buffer,  "(" );
                buffer++;
            }

            lim = cur + cur->intval;
            cur++;
            while ( cur < lim )
            {
                encode( cur, TRUE );
                buffer += strlen( buffer );

                cur += cur->intval;

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
        ABORT;

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
        ABORT;

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


static void
apply__encode__alt( Apply *a, char *buffer )
{
    Object *o;
    Name *name;

    if ( DEBUG__SAFE && ( !a || !buffer ) )
        ABORT;

    o = a->function;
    encode__short( o, buffer );
    buffer += strlen( buffer );

    sprintf( buffer, " " );
    buffer++;

    o = a->operand;

    if ( DEBUG__SAFE && !FIRST_CLASS_NULL && !o )
        ABORT;

    /* If the operand is another Apply (and does not have a name),
       enclose it in parentheses. */
    if ( ( FIRST_CLASS_NULL && o ) && object__type( o ) == interpreter->apply_t )
    {
        name = encoding_name( o );
        if ( name )
        {
            name__delete( name );
            encode__short( o, buffer );
        }

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
resolve( Interpreter *itp, Object *o )
{
    ACTION helper( Object **opp )
    {
        Object *o = DEREF( opp );

        if ( FIRST_CLASS_NULL && !o )
            return CONTINUE;

        Type *t = object__type( o );

        if ( t == itp->name_t )
        {
            o = interpreter__resolve( itp, object__value( o ) );
            *opp = o;
        }

        else if ( t->flags & TYPE__IS_OBJ_COLL )
            t->walk( object__value( o ), ( Visitor ) helper );

        return CONTINUE;
    }

    Object **opp = malloc( sizeof ( Object* ) );
    *opp = o;
    helper( opp );
    o = *opp;
    free( opp );
    return o;
}


/******************************************************************************/


int
interpreter__evaluate_expression( Interpreter *itp,
                                  OBJ( NAME ) *name,
                                  Object *expr,
                                  const char *text )
{
    int ret = 0;
    Object *o;
    char print_buffer[ENCODING__BUFFER_SIZE];
    Name *oname = 0;
    Array *spine;

    Encoder
        apply__encode,
        bag__encode,
        char__encode,
        double__encode,
        string__encode,
        set__encode,
        term__encode;

    /* See: http://www.gnu.org/prep/standards/standards.html#Conditional-Compilation */
    if ( DEBUG__SAFE && !expr )
        ABORT;

    /* FIXME */
    interpreter = itp;

    interpreter__add_to_history( text );

    apply__encode = itp->apply_t->encode;
    bag__encode = itp->bag_t->encode;
    char__encode = itp->char_t->encode;
    double__encode = itp->float_t->encode;
    string__encode = itp->string_t->encode;
    set__encode = itp->set_t->encode;
    term__encode = itp->term_t->encode;

    itp->apply_t->encode = ( Encoder ) apply__encode__alt;
    itp->bag_t->encode = ( Encoder ) array__encode__alt;
    itp->char_t->encode = ( Encoder ) char__encode__alt;
    itp->float_t->encode = ( Encoder ) double__encode__alt;
    itp->string_t->encode = ( Encoder ) string__encode__alt;
    itp->set_t->encode = ( Encoder ) set__encode__alt;
    itp->term_t->encode = ( Encoder ) term__encode__alt;

    o = resolve( itp, expr );

    /* If a term, reduce. */
    if ( o && object__type( o ) == itp->apply_t )
    {
        if ( COMPILER__REDUCE_AS_GRAPH )
        {
            spine = array__new( 0, 0 );
            o = reduce__graph_lazy( o, spine, environment__manager( itp->env ) );
            array__delete( spine );
        }

        else
            ERROR( "term reduction not supported by Interpreter" );
    }

    if ( o )
    {
        if ( name )
            interpreter__define( itp, object__value( name ), o );

        /* Command-line output. */
        if ( !itp->quiet )
        {
            oname = interpreter__name_of__full( itp, itp->cur_ns_obj, o );

            if ( COMPILER__SHOW_ADDRESS )
            {
                PRINT( "%p ", ( void* ) o ); FFLUSH;
            }

            PRINT( "<%s> ", object__type( o )->name );

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

    if ( oname )
        name__delete( oname );

    itp->apply_t->encode = apply__encode;

    itp->bag_t->encode = bag__encode;
    itp->char_t->encode = char__encode;
    itp->float_t->encode = double__encode;
    itp->string_t->encode = string__encode;
    itp->set_t->encode = set__encode;
    itp->term_t->encode = term__encode;

    manager__collect( environment__manager( itp->env ), FALSE, FALSE );

    return ret;
}


/******************************************************************************/


#define DRAW_NAMED_OBJECTS  0

char *
interpreter__draw( Interpreter *c, const Object *o )
{
    int topid = 0;
    char buffer[ENCODING__BUFFER_SIZE];
    char *cur = buffer;

    const char *function_color = "#C8C0FF";
    const char *atom_color = "#E0E0FF";

    int draw( const Object *o, boolean force )
    {
        int id = topid++;

        Type *t;
        Apply *a;
        int left, right;
        char *prefix;
        Name *name;

        sprintf( cur, "%i [label=\"", id );
        cur += strlen( cur );

        if ( !o )
        {
            sprintf( cur, "()" );
            cur += 2;
        }

        else
        {
            t = object__type( o );

            prefix = ( t == c->apply_t )
                ? "@" : ( t == c->indirection_t )
                ? "->" : "";

            sprintf( cur, prefix );
            cur += strlen( cur );

            name = interpreter__name_of( c, 0, o );
            if ( name )
            {
/*
                sprintf( cur, " " );
                cur++;
*/

                name__encode( name, cur );
                cur += strlen( cur );

                name__delete( name );
            }

            else if ( t != c->apply_t )
            {
                /* FIXME: beware of overflow, and of quote characters */
                object__encode( o, cur );
                cur += strlen( cur );
            }

            sprintf( cur, "\"" );
            cur++;

            if ( c->apply_t == t )
            {
                sprintf( cur, ", color=\"%s\"", function_color );
                cur += strlen( cur );
            }

            sprintf( cur, "];\n" );
            cur += strlen( cur );

            if ( !name || DRAW_NAMED_OBJECTS || force )
            {
                if ( t == c->apply_t )
                {
                    a = object__value( o );
                    left = draw( a->function, FALSE );
                    right = draw( a->operand, FALSE );
                    sprintf( cur, "%i -> %i;\n", id, left );
                    cur += strlen( cur );
                    sprintf( cur, "%i -> %i;\n", id, right );
                    cur += strlen( cur );
                }

                else if ( t == c->indirection_t )
                {
                    left = draw( object__value( o ), FALSE );
                    sprintf( cur, "%i --> %i;\n", id, left );
                    cur += strlen( cur );
                }
            }
        }

        return id;
    }

    sprintf( cur, "digraph Phase2Object {\n" ); cur += strlen( cur );
    sprintf( cur, "graph [nodesep=.3, ranksep=.3, concentrate=true]\n" ); cur += strlen( cur );
    sprintf( cur, "node [shape=box, style=\"rounded, filled\", color=\"%s\" width=.3, height=.3, fontname=\"Times-Bold\", fontsize=10, peripheries=0, labeldistance=10]\n", atom_color ); cur += strlen( cur );
    sprintf( cur, "edge [arrowhead=normal, arrowsize=.6];\n" ); cur += strlen( cur );

    draw( o, TRUE );
    sprintf( cur, "}\n" );
    cur += strlen( cur );

    return STRDUP( buffer );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
