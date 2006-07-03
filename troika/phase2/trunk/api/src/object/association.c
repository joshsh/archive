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

#include "Object-impl.h"


#if TRIPLES__GLOBAL

Object *
object__multiply( Object *subj, Object *pred )
{
    Object *obj;

    if ( DEBUG__SAFE && ( !subj || !pred ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    obj = ( subj->outbound_edges )
        ? hash_map__lookup( subj->outbound_edges, pred )
        : 0 ;

#else
    obj = 0;
#endif

    return obj;
}


Object *
object__associate( Object *subj, Object *pred, Object *obj )
{
    if ( DEBUG__SAFE && ( !subj || !pred || !obj ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( !subj->outbound_edges
      && !( subj->outbound_edges = hash_map__new() ) )
        subj = 0;

    else
    {
        if ( obj )
            hash_map__add( subj->outbound_edges, pred, obj );

        else
            hash_map__remove( subj->outbound_edges, pred );
    }
#else
    subj = 0;
#endif

    return subj;
}


Object *
object__dissociate( Object *subj, Object *pred )
{

    if ( DEBUG__SAFE && ( !subj || !pred ) )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( subj->outbound_edges )
    {
        hash_map__remove( subj->outbound_edges, pred );
        if ( !hash_table__size( subj->outbound_edges ) )
        {
            hash_map__delete( subj->outbound_edges );
            subj->outbound_edges = 0;
        }
    }
#else
    subj = 0;
#endif

    return subj;
}


#ifdef NOT_FINISHED
static Object *
union_of( Object *o1, Object *o2 )
{
    Set *s;
    Object *o3;

    if ( o1 )
    {
        if ( o1->type == set_t )
        {
            if ( o2 )
            {
                /* A + A */
                if ( o2->type == set_t )
                {

                }

                /* A + a */
                else
                {

                }
            }

            /* A + 0 */
            else
            {
                set__remove( s, o2 );
            }
        }

        else if ( o2 )
        {
            /* a + A */
            if ( o2->type == set_t )
            {

            }

            /* a + a */
            else
            {
                s = set__new();
                set__add( s, o1 );
                set__add( s, o2 );
            }
        }

        /* a + 0 */
        else
        {

        }
    }

    else
    {
        if ( o2 )
        {
            /* 0 + A */
            if ( o2->type == set_t )
            {
                s = set__copy( o2->value );
                o3 = memory_manager__object( environment__manager( global_env ), set_t, s, 0 );
                return o3;
            }

            /* 0 + a */
            else
                return o2;
        }

        /* 0 + 0 */
        else
            return 0;
    }
}


Object *
object__union_associate( Object *subj, Object *pred, Object *obj )
{
    Object *o;

    if ( DEBUG__SAFE && ( !subj || !pred || !obj )
        abort();

#if TRIPLES__GLOBAL__OUT_EDGES
    if ( !( subj->outbound_edges || ( subj->outbound_edges = hash_map__new() ) ) )
        subj = 0;

    else
    {
        o = union_of( hash_map__lookup( subj->outbound_edges, pred ), obj );
        hash_map__add( subj->outbound_edges, pred, o );
    }
#else
    subj = 0;
#endif

    return subj;
}
#endif

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
