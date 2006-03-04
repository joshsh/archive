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

} p2_search;


static p2_procedure__effect break_true_if_match
    ( void **addr, p2_search *search )
{
    if ( search->match( *addr ) )
    {
        search->single_result = *addr;
        return p2_procedure__effect__break;
    }

    else
        return p2_procedure__effect__continue;
}


static p2_procedure__effect break_false_if_nomatch
    ( void **addr, p2_search *search )
{
    if ( search->match( *addr ) )
        return p2_procedure__effect__continue;

    else
    {
        search->single_result = 0;
        return p2_procedure__effect__break;
    }
}


/* do_for_all *****************************************************************/


typedef struct _void_f_wrapper
{
    void_f f;

} void_f_wrapper;


static p2_procedure__effect void_f_procedure
    ( void **addr, void_f_wrapper *wrapper )
{
    wrapper->f( *addr );

    return p2_procedure__effect__continue;
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


static p2_procedure__effect exclude_if_match
    ( void **addr, p2_search *search )
{
    return ( search->match( *addr ) )
        ? p2_procedure__effect__remove
        : p2_procedure__effect__continue;
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
static p2_procedure__effect add_if_match( void **addr, p2_search *search )
{
    if ( search->match( *addr ) )
    {
        p2_array__enqueue( search->results, *addr );
    }

    return p2_procedure__effect__continue;
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

} substitution_context;


static p2_procedure__effect substitute
    ( void **addr, substitution_context *context )
{
    *addr = context->substitute_for( *addr, context->state );

    return p2_procedure__effect__continue;
}


void p2_collection__replace_all( p2_collection *c, substitution_f f, void *state )
{
    p2_procedure p;
    substitution_context context;

    context.substitute_for = f;
    context.state = state;

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
