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

#include <collection/Collection.h>
#include "object/Object-impl.h"


/* do_for_all *****************************************************************/


void
collection__do_for_all( Collection *c, Void_f f )
{
    void *helper( void **refp )
    {
        f( *refp );
        return 0;
    }

    if ( DEBUG__SAFE && ( !c || !f ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );
}


/* exclude if *****************************************************************/


void
collection__exclude_if( Collection *c, Criterion cr )
{
    void *helper( void **refp )
    {
        if ( cr( *refp ) )
            return walker__remove;
        else
            return 0;
    }

    if ( DEBUG__SAFE && ( !c || !cr ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );
}


/* exists *********************************************************************/


boolean
collection__exists( Collection *c, Criterion cr )
{
    boolean exists = FALSE;

    void *helper( void **refp )
    {
        if ( cr( *refp ) )
        {
            exists = TRUE;
            return walker__break;
        }

        else
            return 0;
    }

    if ( DEBUG__SAFE && ( !c || !cr ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );

    return exists;
}


/* first_match ****************************************************************/


void *
collection__first_match( Collection *c, Criterion cr )
{
    void *result = 0;

    void *helper( void **refp )
    {
        if ( cr( *refp ) )
        {
            result = *refp;
            return walker__break;
        }

        else
            return 0;
    }

    if ( DEBUG__SAFE && ( !c || !cr ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );

    return result;
}


/* for_all ********************************************************************/


boolean
collection__for_all( Collection *c, Criterion cr )
{
    boolean result = TRUE;

    void *helper( void **refp )
    {
        if ( !cr( *refp ) )
        {
            result = FALSE;
            return walker__break;
        }

        else
            return 0;
    }

    if ( DEBUG__SAFE && ( !c || !cr ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );

    return result;
}


/* match **********************************************************************/


Array *
collection__match( Collection *c, Criterion cr )
{
    Array *results = array__new( 0, 0 );

    void *helper( void **refp )
    {
        if ( cr( *refp ) )
            array__enqueue( results, *refp );
        return 0;
    }

    if ( DEBUG__SAFE && ( !c || !cr ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );

    return results;
}


/* replace_all ****************************************************************/


void
collection__replace_all( Collection *c, Substitution f )
{
    void *helper( void **refp )
    {
        *refp = f( *refp );
        return 0;
    }

    if ( DEBUG__SAFE && ( !c || !f ) )
        abort();

    c->type->walk( c->value, ( Dist_f ) helper );
}


/* sort ***********************************************************************/


void
collection__sort( Collection *c, Comparator cmp )
{
    if ( DEBUG__SAFE && ( !c || !cmp ) )
        abort();

    c->type->sort( c->value, cmp );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
