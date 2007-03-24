package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.ListNode;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

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

	public RippleList evaluate( Interpreter itp, ModelConnection mc )
		throws RippleException
	{
//        boolean comp = ( Ripple.getEvaluationStyle() == Ripple.EvaluationStyle.COMPOSITIONAL );

		ListNode<Ast> in = reorder();
		RippleList out = null;

		while ( null != in )
		{
			out = new RippleList( in.getFirst().evaluate( itp, mc ), out );
			in = in.getRest();
		}

		return out;
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
}

// kate: tab-width 4
