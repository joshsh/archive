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


typedef struct Search_Ctx Search_Ctx;

struct Search_Ctx
{
    Criterion match;
    Array *results;
    void *single_result;

    p2_action action;
};


static p2_action *
break_true_if_match( void *data, Search_Ctx *search )
{
    if ( search->match( data ) )
    {
        search->single_result = data;
        search->action.type = p2_action__type__break;
        return &search->action;
    }

    else
        return 0;
}


static p2_action *
break_false_if_nomatch( void *data, Search_Ctx *search )
{
    if ( search->match( data ) )
        return 0;

    else
    {
        search->single_result = 0;
        search->action.type = p2_action__type__break;
        return &search->action;
    }
}


/* do_for_all *****************************************************************/


typedef struct Voidf_Ctx Voidf_Ctx;

struct Voidf_Ctx
{
    Void_f f;
};


static p2_action *
Void_f_procedure( void *data, Voidf_Ctx *wrapper )
{
    wrapper->f( data );

    return 0;
}


void
collection__do_for_all( Collection *c, Void_f f )
{
    Closure p;
    Voidf_Ctx ctx;

    ctx.f = f;

    p.execute = ( procedure ) Void_f_procedure;
    p.state = &ctx;

    c->type->distribute( c->value, &p );
}


/* exclude_if *****************************************************************/


static p2_action *
exclude_if_match( void *data, Search_Ctx *search )
{
    if ( search->match( data ) )
    {
        search->action.type = p2_action__type__remove;
        return &search->action;
    }

    else
        return 0;
}


void
collection__exclude_if( Collection *c, Criterion cr )
{
    Closure p;
    Search_Ctx search;

    search.match = cr;

    p.execute = ( procedure ) exclude_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
}


/* exists *********************************************************************/


boolean
collection__exists( Collection *c, Criterion cr )
{
    Closure p;
    Search_Ctx search;

    search.match = cr;
    search.single_result = ( void* ) 0;

    p.execute = ( procedure ) break_true_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return ( search.single_result ) ? boolean__true : boolean__false;
}


/* first_match ****************************************************************/


void *
collection__first_match( Collection *c, Criterion cr )
{
    Closure p;
    Search_Ctx search;

    search.match = cr;
    search.single_result = 0;

    p.execute = ( procedure ) break_true_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return search.single_result;
}


/* for_all ********************************************************************/


boolean
collection__for_all( Collection *c, Criterion cr )
{
    Closure p;
    Search_Ctx search;

    search.match = cr;
    search.single_result = ( void* ) 1;

    p.execute = ( procedure ) break_false_if_nomatch;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return ( search.single_result ) ? boolean__true : boolean__false;
}


/* match **********************************************************************/


/* Procedure for pattern matching. */
static p2_action *
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
    Closure p;
    Search_Ctx search;

    search.match = cr;
    search.results = array__new( 0, 0 );

    p.execute = ( procedure ) add_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return search.results;
}


/* replace_all ****************************************************************/


typedef struct Subst_Ctx Subst_Ctx;

struct Subst_Ctx
{
    Substitution substitute_for;

    void *state;
    p2_action action;
};


static p2_action *
substitute( void *data, Subst_Ctx *context )
{
    context->action.value = context->substitute_for( data, context->state );

    return &context->action;
}


void
collection__replace_all( Collection *c, Substitution f, void *state )
{
    Closure p;
    Subst_Ctx context;

    context.substitute_for = f;
    context.state = state;
    context.action.type = p2_action__type__replace;

    p.execute = ( procedure ) substitute;
    p.state = &context;

    c->type->distribute( c->value, &p );
}


/* sort ***********************************************************************/


void
collection__sort( Collection *c, Comparator cmp )
{
    c->type->sort( c->value, cmp );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
