/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.ListNode;

// TODO: this class has more plumbing than it needs
public class ListAst extends ListNode<Ast> implements Ast<RippleList>
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

	public void evaluate( final Sink<RippleList, RippleException> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		Sink<RippleList, RippleException> listSink = new Sink<RippleList, RippleException>()
		{
			public void put(final RippleList l) throws RippleException
			{
				sink.put( mc.list( l ) );
			}
		};

		createLists( this, listSink, qe, mc );
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
							final Sink<RippleList, RippleException> sink,
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
			final Collector<RippleList, RippleException> firstValues = new Collector<RippleList, RippleException>();
			astList.getFirst().evaluate( firstValues, qe, mc );
	
			Sink<RippleList, RippleException> restSink = new Sink<RippleList, RippleException>()
			{
				public void put( final RippleList rest ) throws RippleException
				{
					Sink<RippleList, RippleException> firstSink = new Sink<RippleList, RippleException>()
					{
						public void put( final RippleList f ) throws RippleException
						{
							sink.put( mc.concat( f, rest ) );
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
