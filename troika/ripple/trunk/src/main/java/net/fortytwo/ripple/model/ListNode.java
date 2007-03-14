package net.fortytwo.ripple.model;

public abstract class ListNode<T>
{
    public abstract T getFirst();
    public abstract ListNode<T> getRest();

    public boolean equals( ListNode<T> other )
    {
System.out.println( this + ".equals( " + other + " )" );
        ListNode thisCur = this;
        ListNode otherCur = other;

        while ( null != thisCur )
        {
            if ( null == otherCur )
                return false;

            if ( !thisCur.getFirst().equals( otherCur ) )
                return false;

            thisCur = thisCur.getRest();
            otherCur = otherCur.getRest();
        }

        if ( null != otherCur )
            return false;

        return true;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
