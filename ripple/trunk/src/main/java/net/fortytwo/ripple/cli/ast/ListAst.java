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
import net.fortytwo.ripple.util.ListNode;
import net.fortytwo.ripple.util.Sink;

// TODO: this class has more plumbing than it needs
public class ListAst extends ListNode<Ast> implements Ast
{
	private Ast first;
	private ListNode<Ast> rest;

	private ModelConnection modelConnection;
	private QueryEngine queryEngine;
	private Sink<RippleValue> valueSink;

	public Ast getFirst()
	{
		return first;
	}

	public ListNode<Ast> getRest()
	{
		return rest;
	}

	/**
	 *  Constructs a nil list AST.
	 */
	public ListAst()
	{
		first = null;
	}

	public ListAst( final Ast first, final ListNode<Ast> rest )
	{
//System.out.println( "first = " + first + ", rest = " + rest );
		this.first = first;
		this.rest = rest;
	}

	private boolean isNil()
	{
		return ( null == first );
	}

/*
	private ListNode<Ast> reorder()
	{
		// Note: assuming left associativity for now.
		switch ( Ripple.getExpressionOrder() )
		{
			case DIAGRAMMATIC:
				return this;

			case ANTIDIAGRAMMATIC:
				ListNode<Ast> in = this, out = null;

				while ( null != in )
				{
					out = new ListAst( in.getFirst(), out );
					in = in.getRest();
				}

				return out;
		}
return null;
	}
*/

	private Sink<RippleList> getSink( final ListNode<Ast> listAst )
	{
		if ( null == listAst )
		{
			return new ListToValueSink( valueSink );
		}

		else
		{
			return new ListBuilderSink( listAst.getFirst(),
				getSink( listAst.getRest() ) );
		}
	}

	public void evaluate( final Sink<RippleValue> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		if ( isNil() )
		{
//			return;
			sink.put( RippleList.NIL );
		}

//        boolean comp = ( Ripple.getEvaluationStyle() == Ripple.EvaluationStyle.COMPOSITIONAL );
		modelConnection = mc;
		queryEngine = qe;
		valueSink = sink;
		
		Sink<RippleList> listSink = getSink( this );
		listSink.put( RippleList.NIL );
/*
		ListNode<Ast> in = reorder();
		RippleList out = null;

		while ( null != in )
		{
			out = new RippleList( in.getFirst().evaluate( itp, mc ), out );
			in = in.getRest();
		}

		return out;
*/
	}

	public String toString()
	{
		StringBuilder s = new StringBuilder();
		s.append( "(" );

		if ( !isNil() )
		{
			ListNode<Ast> cur = this;
			boolean first = true;
			while ( null != cur )
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
		}

		s.append( ")" );
		return s.toString();
	}

	private class ListBuilderSink implements Sink<RippleList>
	{
		private Sink<RippleList> sink;
		private Ast ast;

		private class InnerSink implements Sink<RippleValue>
		{
			private RippleList tail;
	
			public InnerSink( final RippleList list )
			{
				tail = list;
			}
	
			public void put( final RippleValue v )
				throws RippleException
			{
				sink.put( new RippleList( v, tail ) );
			}
		}

		ListBuilderSink( final Ast ast, final Sink<RippleList> sink )
		{
			this.sink = sink;
			this.ast = ast;
		}

		public void put( final RippleList list ) throws RippleException
		{
//System.out.println( "ast = " + ast );
			InnerSink innerSink = new InnerSink( list );

			if ( null == ast )
			{
				innerSink.put( RippleList.NIL );
			}

			else
			{
				ast.evaluate( innerSink, queryEngine, modelConnection );
			}
		}
	}

	private class ListToValueSink implements Sink<RippleList>
	{
		private Sink<RippleValue> sink;

		public ListToValueSink( final Sink<RippleValue> sink )
		{
			this.sink = sink;
		}

		public void put( final RippleList list ) throws RippleException
		{
			sink.put( list );
		}
	}
}

// kate: tab-width 4
