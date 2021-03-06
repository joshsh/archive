package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.ListNode;
import net.fortytwo.ripple.util.Sink;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

// TODO: this class has more plumbing than it needs
public class ListAst extends ListNode<Ast> implements Ast
{
	private Ast first;
	private ListNode<Ast> rest;

	public Ast getFirst()
	{
		return first;
	}

	public ListNode<Ast> getRest()
	{
		return rest;
	}

	public ListAst( Ast first, ListNode<Ast> rest )
	{
//System.out.println( "first = " + first + ", rest = " + rest );
		this.first = first;
		this.rest = rest;
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

	private ModelConnection modelConnection;
	private Interpreter interpreter;
	private Sink<RippleValue> valueSink;

	private Sink<RippleList> getSink( ListNode<Ast> listAst )
	{
		if ( null == listAst )
			return new ListToValueSink( valueSink );
		else
			return new ListBuilderSink( listAst.getFirst(),
				getSink( listAst.getRest() ) );
	}

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
//        boolean comp = ( Ripple.getEvaluationStyle() == Ripple.EvaluationStyle.COMPOSITIONAL );
		modelConnection = mc;
		interpreter = itp;
		valueSink = sink;
		
		Sink<RippleList> listSink = getSink( this );
		listSink.put( null );
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
		ListNode<Ast> cur = this;
		boolean first = true;
		while ( null != cur )
		{
			if ( first )
				first = false;
			else
				s.append( " " );

			s.append( cur.getFirst().toString() );
			cur = cur.getRest();
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
	
			public InnerSink( RippleList list )
			{
				tail = list;
			}
	
			public void put( RippleValue v )
				throws RippleException
			{
				sink.put( new RippleList( v, tail ) );
			}
		}

		ListBuilderSink( Ast ast, Sink<RippleList> sink )
		{
			this.sink = sink;
			this.ast = ast;
		}

		public void put( RippleList list )
			throws RippleException
		{
//System.out.println( "ast = " + ast );
			InnerSink innerSink = new InnerSink( list );
			ast.evaluate( innerSink, interpreter, modelConnection );
		}
	}

	private class ListToValueSink implements Sink<RippleList>
	{
		private Sink<RippleValue> sink;

		public ListToValueSink( Sink<RippleValue> sink )
		{
			this.sink = sink;
		}

		public void put( RippleList list )
			throws RippleException
		{
			sink.put( list );
		}
	}
}

// kate: tab-width 4
