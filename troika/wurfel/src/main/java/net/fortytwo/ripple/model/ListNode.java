package net.fortytwo.ripple.model;

import net.fortytwo.ripple.model.Node;

public class ListNode<T> extends Node
{
    private T first;
    private ListNode<T> rest;

    public T getFirst()
    {
        return first;
    }

    public ListNode<T> getRest()
    {
        return rest;
    }

    public ListNode( final T first )
    {
        this.first = first;
        rest = null;
    }

    public ListNode( final T first, final ListNode<T> rest )
    {
        this.first = first;
        this.rest = rest;
    }

    public ListNode push( final T first )
    {
        return new ListNode( first, this );
    }

    public boolean equals( ListNode<T> other )
    {
        ListNode thisCur = this;
        ListNode otherCur = other;

        while ( null != thisCur )
        {
            if ( null == otherCur )
                return false;

            if ( !thisCur.first.equals( otherCur ) )
                return false;

            thisCur = thisCur.rest;
            otherCur = otherCur.rest;
        }

        if ( null != otherCur )
            return false;

        return true;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
