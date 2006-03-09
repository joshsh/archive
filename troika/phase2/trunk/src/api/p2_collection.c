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

#include "p2_collection.h"


typedef struct _p2_search
{
    criterion match;
    p2_array *results;
    void *single_result;

    p2_action action;

} p2_search;


static p2_action * break_true_if_match
    ( void *data, p2_search *search )
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


static p2_action * break_false_if_nomatch
    ( void *data, p2_search *search )
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


typedef struct _void_f_wrapper
{
    void_f f;

} void_f_wrapper;


static p2_action * void_f_procedure
    ( void *data, void_f_wrapper *wrapper )
{
    wrapper->f( data );

    return 0;
}


void p2_collection__do_for_all( p2_collection *c, void_f f )
{
    p2_procedure p;
    void_f_wrapper wrapper;

    wrapper.f = f;

    p.execute = ( procedure ) void_f_procedure;
    p.state = &wrapper;

    c->type->distribute( c->value, &p );
}


/* exclude_if *****************************************************************/


static p2_action * exclude_if_match
    ( void *data, p2_search *search )
{
    if ( search->match( data ) )
    {
        search->action.type = p2_action__type__remove;
        return &search->action;
    }

    else
        return 0;
}


void p2_collection__exclude_if( p2_collection *c, criterion cr )
{
    p2_procedure p;
    p2_search search;

    search.match = cr;

    p.execute = ( procedure ) exclude_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
}


/* exists *********************************************************************/


boolean p2_collection__exists( p2_collection *c, criterion cr )
{
    p2_procedure p;
    p2_search search;

    search.match = cr;
    search.single_result = ( void* ) 0;

    p.execute = ( procedure ) break_true_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return ( search.single_result ) ? boolean__true : boolean__false;
}


/* first_match ****************************************************************/


void *p2_collection__first_match( p2_collection *c, criterion cr )
{
    p2_procedure p;
    p2_search search;

    search.match = cr;
    search.single_result = 0;

    p.execute = ( procedure ) break_true_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return search.single_result;
}


/* for_all ********************************************************************/


boolean p2_collection__for_all( p2_collection *c, criterion cr )
{
    p2_procedure p;
    p2_search search;

    search.match = cr;
    search.single_result = ( void* ) 1;

    p.execute = ( procedure ) break_false_if_nomatch;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return ( search.single_result ) ? boolean__true : boolean__false;
}


/* match **********************************************************************/


/* Procedure for pattern matching. */
static p2_action * add_if_match( void *data, p2_search *search )
{
    if ( search->match( data ) )
    {
        p2_array__enqueue( search->results, data );
    }

    return 0;
}


p2_array *p2_collection__match( p2_collection *c, criterion cr )
{
    p2_procedure p;
    p2_search search;

    search.match = cr;
    search.results = p2_array__new( 0, 0 );

    p.execute = ( procedure ) add_if_match;
    p.state = &search;

    c->type->distribute( c->value, &p );
    return search.results;
}


/* replace_all ****************************************************************/


typedef struct _substitution_context
{
    substitution_f substitute_for;

    void *state;
    p2_action action;

} substitution_context;


static p2_action * substitute
    ( void *data, substitution_context *context )
{
    context->action.value = context->substitute_for( data, context->state );

    return &context->action;
}


void p2_collection__replace_all( p2_collection *c, substitution_f f, void *state )
{
    p2_procedure p;
    substitution_context context;

    context.substitute_for = f;
    context.state = state;
    context.action.type = p2_action__type__replace;

    p.execute = ( procedure ) substitute;
    p.state = &context;

    c->type->distribute( c->value, &p );
}


/* sort ***********************************************************************/


void p2_collection__sort( p2_collection *c, comparator cmp )
{
    c->type->sort( c->value, cmp );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
