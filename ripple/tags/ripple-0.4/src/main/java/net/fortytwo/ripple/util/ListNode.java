/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

/**
 * Head of a linked list.
 */
public abstract class ListNode<T>
{
	public abstract T getFirst();
	public abstract ListNode<T> getRest();
	
	public boolean equals( final ListNode<T> other )
	{
		ListNode thisCur = this;
		ListNode otherCur = other;
	
		while ( null != thisCur )
		{
			if ( null == otherCur )
			{
				return false;
			}

			if ( !thisCur.getFirst().equals( otherCur.getFirst() ) )
			{
				return false;
			}
	
			thisCur = thisCur.getRest();
			otherCur = otherCur.getRest();
		}
	
		if ( null != otherCur )
		{
			return false;
		}
	
		return true;
	}
}

// kate: tab-width 4
