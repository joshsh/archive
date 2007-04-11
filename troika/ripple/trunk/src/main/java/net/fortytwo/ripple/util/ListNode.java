package net.fortytwo.ripple.util;

public abstract class ListNode<T>
{
	public abstract T getFirst();
	public abstract ListNode<T> getRest();
	
	public boolean equals( ListNode<T> other )
	{
System.out.println( "[" + this + "].equals(" + other + ")" );
		ListNode thisCur = this;
		ListNode otherCur = other;
	
		while ( null != thisCur )
		{
			if ( null == otherCur )
{
System.out.println( "null == otherCur" );
				return false;
}

			if ( !thisCur.getFirst().equals( otherCur.getFirst() ) )
{
System.out.println( "!thisCur.getFirst().equals( otherCur )" );
				return false;
}
	
			thisCur = thisCur.getRest();
			otherCur = otherCur.getRest();
		}
	
		if ( null != otherCur )
{
System.out.println( "null != otherCur" );
			return false;
}
	
		return true;
	}
}

// kate: tab-width 4
