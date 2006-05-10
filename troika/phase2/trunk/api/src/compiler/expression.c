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

#include <sk/sk.h>

#include "Compiler-impl.h"


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
term__encode__alt( Term *t, char *buffer )
{
/*
    sprintf( buffer, "[" );
    buffer++;
*/
    term__encode( t, buffer );
/*
    buffer += strlen( buffer );
    sprintf( buffer, "]" );
*/
}


/******************************************************************************/


static Object *
resolve( Ast *ast, Compiler *c )
{
    /* Transforms an Ast into an Object, deleting the Ast along the way. */
    Object *object_for_ast( Ast* ast )
    {
        boolean ok = TRUE;

        void *helper( Ast **astpp )
        {
            if ( !( *astpp = ( Ast* ) object_for_ast( *astpp ) ) )
            {
                ok = FALSE;
                return walker__break;
            }

            else
                return 0;
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
                o = compiler__resolve( c, ( Name* ) ast->value );
                ast__delete( ast );
                return o;

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

            #if DEBUG__SAFE
            default:

                ERROR( "object_for_ast: bad AST type" );
                free( ast );
                return 0;
            #endif
        }

        free( ast );

        if ( ok )
        {
            /* Create and register a new object. */
            o = object__new( type, value, flags );

            memory_manager__add( environment__manager( c->env ), o );

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
compiler__evaluate_expression( Compiler *c, Name *name, Ast *expr )
{
    int ret = 0;
    Ast *a = 0;
    Object *o;
    char print_buffer[ENCODING__BUFFER_SIZE];
    Name *oname = 0;
    Term *t;

    Encoder char__encode, double__encode, string__encode, term__encode;

    /* See: http://www.gnu.org/prep/standards/standards.html#Conditional-Compilation */
    if ( DEBUG__SAFE && !expr )
        abort();

    char__encode = c->char_t->encode;
    double__encode = c->float_t->encode;
    string__encode = c->string_t->encode;
    term__encode = c->term_t->encode;
    c->char_t->encode = ( Encoder ) char__encode__alt;
    c->float_t->encode = ( Encoder ) double__encode__alt;
    c->string_t->encode = ( Encoder ) string__encode__alt;
    c->term_t->encode = ( Encoder ) term__encode__alt;

    if ( name )
        a = ast__name( name );

    o = resolve( expr, c );

    /* If a term, reduce. */
    if ( o && o->type == c->term_t )
    {
        t = sk_reduce( ( Term* ) o->value,
            environment__manager( c->env ),
            c->term_t,
            c->prim_t,
            c->combinator_t );

        if ( t )
            o->value = t;

        else
            o = 0;
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
            compiler__define( c, name, o );

        oname = namespace__find( c->cur_ns_obj, o, environment__manager( c->env ) );
        if ( !oname )
        {
            oname = namespace__find( environment__root( c->env ), o, environment__manager( c->env ) );

            /* ~ */
            if ( oname )
                array__push( oname, STRDUP( "root" ) );
        }

        /* Command-line output. */
        if ( !c->quiet )
        {
            if ( COMPILER__SHOW_ADDRESS )
            {
                printf( "%#x ", ( int ) o ); FFLUSH;
            }

            printf( "<%s> ", o->type->name );

            if ( oname )
            {
                name__print( oname );
                printf( " : " );
            }

            else
                printf( ": " );

            o->type->encode( o->value, print_buffer );
            printf( print_buffer );

            printf( "\n" );
        }
    }

    if ( a )
        ast__delete( a );

    if ( oname )
        name__delete( oname );

    c->char_t->encode = char__encode;
    c->float_t->encode = double__encode;
    c->string_t->encode = string__encode;
    c->term_t->encode = term__encode;

    return ret;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
