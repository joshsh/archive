/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

public class ListMemoizer<T extends Comparable,M>
{
	private T first;
	private M memo;

	private ListMemoizer<T,M> left, right;
	private ListMemoizer<T,M> rest;

	public ListMemoizer( final ListNode<T> list, final M memo )
	{
		first = list.getFirst();
		left = null;
		right = null;

		ListNode<T> r = list.getRest();
		if ( null == r )
		{
			rest = null;
			this.memo = memo;
		}

		else
		{
			rest = new ListMemoizer( r, memo );
			this.memo = null;
		}
	}

	public boolean add( final ListNode<T> list, final M memo )
	{
		int cmp = first.compareTo( list.getFirst() );

		if ( 0 == cmp )
		{
			ListNode<T> r = list.getRest();

			if ( null == r )
			{
				if ( null == this.memo )
				{
					this.memo = memo;
					return true;
				}

				else
				{
					return false;
				}
			}

			else
			{
				if ( null == rest )
				{
					rest = new ListMemoizer<T,M>( r, memo );
					return true;
				}

				else
				{
					return rest.add( r, memo );
				}
			}
		}

		else if ( cmp < 0 )
		{
			if ( null == left )
			{
				left = new ListMemoizer<T,M>( list, memo );
				return true;
			}

			else
			{
				return left.add( list, memo );
			}
		}

		else
		{
			if ( null == right )
			{
				right = new ListMemoizer<T,M>( list, memo );
				return true;
			}

			else
			{
				return right.add( list, memo );
			}
		}
	}

	private int compare( final ListMemoizer<T, M> first,
						final ListMemoizer<T, M> second )
	{
		if ( null == first )
		{
			return ( null == second )
				? 0 : -1;
		}

		else if ( null == second )
		{
			return 1;
		}

		else
		{
			return first.compareTo( second );
		}
	}

	public int compareTo( final ListMemoizer<T, M> other )
	{
		int cmp = first.compareTo( other.first );

		if ( 0 != cmp )
		{
			return cmp;
		}

		cmp = compare( left, other.left );
		if ( 0 != cmp )
		{
			return cmp;
		}

		cmp = compare( rest, other.rest );
		if ( 0 != cmp )
		{
			return cmp;
		}

		cmp = compare( right, other.right );
		return cmp;
	}
}

// kate: tab-width 4
