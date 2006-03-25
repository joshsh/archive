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

#include <Collection.h>
#include <Closure.h>


typedef struct Search_Ctx
{
    Criterion match;
    Array *results;
    void *single_result;

} Search_Ctx;


static void *
apply( void *data, Search_Ctx *search )
{
    return ( void* ) search->match( data );
}


static void *
break_true_if_match( void *data, Search_Ctx *search )
{
    return ( void* ) search->match( data );
}


static void *
break_false_if_nomatch( void *data, Search_Ctx *search )
{
    if ( search->match( data ) )
        return 0;

    else
    {
        search->single_result = 0;
        return ( void* ) 1;
    }
}


/* do_for_all *****************************************************************/


typedef struct Voidf_Ctx
{
    Void_f f;

} Voidf_Ctx;


static void *
Void_f_procedure( void *data, Voidf_Ctx *wrapper )
{
    wrapper->f( data );

    return 0;
}


void
collection__do_for_all( Collection *c, Void_f f )
{
    Closure *closure1, *closure2;
    Voidf_Ctx ctx;

    ctx.f = f;

    closure1 = closure__new( ( procedure ) Void_f_procedure, &ctx );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );
}


/* exclude if *****************************************************************/


void
collection__exclude_if( Collection *c, Criterion cr )
{
    Closure *closure1, *closure2;
    Search_Ctx search;

    search.match = cr;

    closure1 = closure__new( ( procedure ) apply, &search );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );
}


/* exists *********************************************************************/


boolean
collection__exists( Collection *c, Criterion cr )
{
    Closure *closure1, *closure2;
    Search_Ctx search;

    search.match = cr;
    search.single_result = ( void* ) 0;

    closure1 = closure__new( ( procedure ) break_true_if_match, &search );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );

    return ( search.single_result ) ? TRUE : FALSE;
}


/* first_match ****************************************************************/


void *
collection__first_match( Collection *c, Criterion cr )
{
    Closure *closure1, *closure2;
    Search_Ctx search;

    search.match = cr;
    search.single_result = 0;

    closure1 = closure__new( ( procedure ) break_true_if_match, &search );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );

    return search.single_result;
}


/* for_all ********************************************************************/


boolean
collection__for_all( Collection *c, Criterion cr )
{
    Closure *closure1, *closure2;
    Search_Ctx search;

    search.match = cr;
    search.single_result = ( void* ) 1;

    closure1 = closure__new( ( procedure ) break_false_if_nomatch, &search );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );

    return ( search.single_result ) ? TRUE : FALSE;
}


/* match **********************************************************************/


/* Procedure for pattern matching. */
static void *
add_if_match( void *data, Search_Ctx *search )
{
    if ( search->match( data ) )
    {
        array__enqueue( search->results, data );
    }

    return 0;
}


Array *
collection__match( Collection *c, Criterion cr )
{
    Closure *closure1, *closure2;
    Search_Ctx search;

    search.match = cr;
    search.results = array__new( 0, 0 );

    closure1 = closure__new( ( procedure ) add_if_match, &search );
    closure2 = closure__cw_dereference( closure1 );
    c->type->distribute( c->value, closure2 );
    closure__delete( closure1 );
    closure__delete( closure2 );

    return search.results;
}


/* replace_all ****************************************************************/


typedef struct Subst_Ctx Subst_Ctx;

struct Subst_Ctx
{
    Substitution substitute_for;

    void *state;
};


static void *
substitute( void **refp, Subst_Ctx *context )
{
    *refp = context->substitute_for( *refp, context->state );
    return 0;
}


void
collection__replace_all( Collection *c, Substitution f, void *state )
{
    Closure *cl;
    Subst_Ctx context;

    context.substitute_for = f;
    context.state = state;

    cl = closure__new( ( procedure ) substitute, &context );
    c->type->distribute( c->value, cl );
    closure__delete( cl );
}


/* sort ***********************************************************************/


void
collection__sort( Collection *c, Comparator cmp )
{
    c->type->sort( c->value, cmp );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
