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
    sprintf( buffer, "[ " );
    buffer += 2;

    term__encode( t, buffer );

    buffer += strlen( buffer );
    sprintf( buffer, " ]" );
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

                type = c->env->bag_t;
                value = ast->value;
                array__walk( value, ( Dist_f ) helper );
                if ( !ok )
                    array__delete( value );
                break;

            case CHAR_T:

                type = c->env->char_t;
                value = ast->value;
                break;

            case FLOAT_T:

                type = c->env->float_t;
                value = ast->value;
                break;

            case INT_T:

                type = c->env->int_t;
                value = ast->value;
                break;

            case NAME_T:

                /* Retrieve an existing object and exit. */
                o = compiler__resolve( c, ( Name* ) ast->value );
                ast__delete( ast );
                return o;

            case STRING_T:

                type = c->env->string_t;
                value = ast->value;
                break;

            case TERM_T:

                type = c->env->term_t;
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

            memory_manager__add( c->env->manager, o );

            return o;
        }

        else
            return 0;
    }

    Object *result = object_for_ast( ast );

    #if DEBUG__COMPILER
    printf( "[%#x] resolve(%#x, %#x)\n",
        ( int ) result, ( int ) ast, ( int ) c );
    #endif

    return result;
}


/******************************************************************************/


int
compiler__evaluate_expression( Compiler *c, Name *name, Ast *expr )
{
    int ret = 0;
    Ast *a = 0;
    Object *o;
    char print_buffer[1000];
    Term *t;

    Encoder char__encode, double__encode, string__encode, term__encode;

    #if DEBUG__SAFE
    if ( !expr )
    {
        ERROR( "compiler__evaluate_expression: null AST node" );
        return 0;
    }
    #endif

    #if DEBUG__COMPILER
    printf( "compiler__evaluate_expression(%#x, %#x, %#x)\n",
        ( int ) c, ( int ) name, ( int ) expr );
    #endif

    char__encode = c->env->char_t->encode;
    double__encode = c->env->float_t->encode;
    string__encode = c->env->string_t->encode;
    term__encode = c->env->term_t->encode;
    c->env->char_t->encode = ( Encoder ) char__encode__alt;
    c->env->float_t->encode = ( Encoder ) double__encode__alt;
    c->env->string_t->encode = ( Encoder ) string__encode__alt;
    c->env->term_t->encode = ( Encoder ) term__encode__alt;

    if ( name )
        a = ast__name( name );

    o = resolve( expr, c );

    /* If a term, reduce. */
    if ( o && o->type == c->env->term_t )
    {
        t = SK_reduce( ( Term* ) o->value,
            c->env->manager,
            c->env->term_t,
            c->env->prim_t,
            c->env->combinator_t );

        if ( t )
            o->value = t;

        else
            o = 0;
    }

    if ( o )
    {
        #if COMPILER__SHOW_ADDRESS
        printf( "%#x ", ( int ) o );
        #endif

        if ( a )
        {
            compiler__define( c, name, o );
            ast__print( a );
            printf( " : " );
        }

        #if COMPILER__SHOW_ADDRESS
        else
            printf( ": " );
        #endif

        printf( "%s  ", o->type->name );

        o->type->encode( o->value, print_buffer );
        printf( print_buffer );

        printf( "\n" );
    }

    if ( a )
        ast__delete( a );

    c->env->char_t->encode = char__encode;
    c->env->float_t->encode = double__encode;
    c->env->string_t->encode = string__encode;
    c->env->term_t->encode = term__encode;

    return ret;
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
