package net.fortytwo.ripple.util;

public class ListMemoizer<T extends Comparable,M>
{
	private T first;
	private M memo;

	private ListMemoizer<T,M> left, right;
	private ListMemoizer<T,M> rest;

	public ListMemoizer( ListNode<T> list, M memo )
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

	public boolean add( ListNode<T> list, M memo )
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
					return false;
			}

			else
			{
				if ( null == rest )
				{
					rest = new ListMemoizer<T,M>( r, memo );
					return true;
				}

				else
					return rest.add( r, memo );
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
				return left.add( list, memo );
		}

		else
		{
			if ( null == right )
			{
				right = new ListMemoizer<T,M>( list, memo );
				return true;
			}

			else
				return right.add( list, memo );
		}
	}
}

// kate: tab-width 4
