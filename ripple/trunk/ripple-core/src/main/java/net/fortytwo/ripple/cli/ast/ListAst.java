/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.ListNode;
import net.fortytwo.ripple.util.Sink;

// TODO: this class has more plumbing than it needs
public class ListAst extends ListNode<Ast> implements Ast
{
	private Ast first;
	private ListNode<Ast> rest;

	/**
	 * Constructs a nil list AST.
	 */
	public ListAst()
	{
//System.out.println( "nil ListAst" );
		first = null;
		rest = null;
	}

	/**
	 * Constructs a list AST with the given first element and rest.
	 */
	public ListAst( final Ast first, final ListNode<Ast> rest )
	{
//System.out.println( "first = " + first + ", rest = " + rest );
		this.first = first;
		this.rest = rest;
	}

	public Ast getFirst()
	{
		return first;
	}

	public ListNode<Ast> getRest()
	{
		return rest;
	}

	private static boolean isNil( final ListNode<Ast> listNode )
	{
		return ( null == listNode.getFirst() );
	}

	public void evaluate( final Sink<RippleValue> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		Sink<RippleList> upcastSink = new Sink<RippleList>()
		{
			public void put( final RippleList list ) throws RippleException
			{
//System.out.println( "putting list: " + list );
				sink.put( list );
			}
		};

		createLists( this, upcastSink, qe, mc );
	}

	public String toString()
	{
		StringBuilder s = new StringBuilder();
		s.append( "(" );

		ListNode<Ast> cur = this;
		boolean first = true;
		while ( !isNil( cur ) )
		{
			if ( first )
			{
				first = false;
			}

			else
			{
				s.append( " " );
			}

			s.append( cur.getFirst().toString() );
			cur = cur.getRest();
		}

		s.append( ")" );
		return s.toString();
	}

	private void createLists( final ListNode<Ast> astList,
							final Sink<RippleList> sink,
							final QueryEngine qe,
							final ModelConnection mc )
		throws RippleException
	{
		if ( isNil( astList ) )
		{
			sink.put( RippleList.NIL );
		}

		else
		{
			final Collector<RippleValue> firstValues = new Collector<RippleValue>();
			astList.getFirst().evaluate( firstValues, qe, mc );
	
			Sink<RippleList> restSink = new Sink<RippleList>()
			{
				public void put( final RippleList rest ) throws RippleException
				{
					Sink<RippleValue> firstSink = new Sink<RippleValue>()
					{
						public void put( final RippleValue f ) throws RippleException
						{
							sink.put( mc.list( f, rest ) );
						}
					};

					firstValues.writeTo( firstSink );
				}
			};

			createLists( astList.getRest(), restSink, qe, mc );
		}
	}
}

// kate: tab-width 4
