/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.Source;

import java.util.Iterator;

/**
 * Note: while this class is not actually thread-safe, put() may safely be
 * called while writeTo() is in progress.
 */
public class Collector<T, E extends Exception> implements Sink<T, E>, Source<T, E>
{
	private Node first, last;
	private int count;

	public Collector()
	{
		clear();
	}

	public void put( final T t ) throws E
	{
		Node n = new Node( t, null );

		if ( null == first )
		{
			first = n;
		}

		else
		{
			last.next = n;
		}

		last = n;
		count++;
	}

	public void writeTo( final Sink<T, E> sink ) throws E
	{
		Node cur = first;
		while ( null != cur )
		{
            try {
                // Any new items which are put() as a result of this call will
                // eventually be passed into the sink as well (even if clear()
                // is called).
                sink.put( cur.value );
            } catch ( Exception e ) {
                System.err.println( "FIXME: exception in Collector.java should not need to be caught" );
                e.printStackTrace( System.err );
            }

            cur = cur.next;
		}
	}

    public int size()
	{
		return count;
	}

	public Iterator<T> iterator()
	{
		return new NodeIterator( first );
	}

	public void clear()
	{
		first = null;
		count = 0;
	}

	////////////////////////////////////////////////////////////////////////////

	private class Node
	{
		public T value;
		public Node next;

		public Node( final T value, final Node next )
		{
			this.value = value;
			this.next = next;
		}
	}

	private class NodeIterator implements Iterator<T>
	{
		private Node cur;

		public NodeIterator( final Node first )
		{
			cur = first;
		}

		public boolean hasNext()
		{
			return null != cur;
		}

		public T next()
		{
			T value = cur.value;
			cur = cur.next;
			return value;
		}

		public void remove()
		{
		}
	}
}

// kate: tab-width 4
