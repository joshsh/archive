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

#include "p2_object.h"


p2_object *p2_object__new( p2_type *type, void *value, int flags )
{
    #if DEBUG__SAFE
        if ( !type || !value)
        {
            PRINTERR( "p2_object__new: null type or value" );
            return 0;
        }
    #endif

    p2_object *o = new( p2_object );

    if ( !o )
        return 0;

    o->type = type;
    o->value = value;
    o->flags = flags;

    #if TRIPLES__GLOBAL__IN_EDGES
        o->inbound_edges = 0;
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
        o->outbound_edges = 0;
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
        o->trans_edges = 0;
    #endif

    return o;
}


void p2_object__delete( p2_object *o )
{
    #if TRIPLES__GLOBAL__IN_EDGES
        if ( o->inbound_edges )
            p2_hash_table__delete( o->inbound_edges );
    #endif

    #if TRIPLES__GLOBAL__OUT_EDGES
        if ( o->outbound_edges )
            p2_hash_table__delete( o->outbound_edges );
    #endif

    #if TRIPLES__GLOBAL__TRANS_EDGES
        if ( o->trans_edges )
            p2_hash_table__delete( o->trans_edges );
    #endif

    /* Free the object's data. */
    o->type->destroy( o->value );

    free( o );
}


/* Member functions ***********************************************************/


p2_object *p2_object__clone( p2_object *o )
{
    /* Caution: flags start at 0. */
    return p2_object__new( o->type, o->type->clone( o->value ), 0 );
}


p2_object *p2_object__decode( p2_type *type, char *buffer )
{
    /* Caution: flags start at 0. */
    return p2_object__new( type, type->decode( buffer ), 0 );
}


void p2_object__encode( p2_object *o, char *buffer )
{
    o->type->encode( o->value, buffer );
}


void *p2_object__exists( p2_object *o, void *(*f)( void* ) )
{
    return o->type->exists( o->value, f );
}


void *p2_object__for_all( p2_object *o, void *(*f)( void* ) )
{
    return o->type->for_all( o->value, f );
}


/* Association ****************************************************************/


#if TRIPLES__GLOBAL

    p2_object *p2_object__multiply
        ( p2_object *subj, p2_object *pred )
    {
        #if TRIPLES__GLOBAL__OUT_EDGES

            return ( subj->outbound_edges ) ?
                ( p2_object* ) p2_hash_table__lookup( subj->outbound_edges, pred ) : 0 ;

        #else

            return 0;

        #endif
    }


    /* Note: doesn't take association sets into account yet. */
    p2_object *p2_object__associate
        ( p2_object *subj, p2_object *pred, p2_object *obj )
    {
        #if TRIPLES__GLOBAL__OUT_EDGES

            if ( !subj->outbound_edges
              && !( subj->outbound_edges = p2_hash_table__new(
                    TRIPLES__GLOBAL__OBJECT_INIT_BUFFER_SIZE, 0, 0,
                    ADDRESS_DEFAULTS ) ) )
                return 0;

            if ( obj )
                p2_hash_table__add( subj->outbound_edges, pred, obj );
            else
                p2_hash_table__remove( subj->outbound_edges, pred );

            return subj;

        #else

            return 0;

        #endif
    }

#endif  /* TRIPLES__GLOBAL */


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
