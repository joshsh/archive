package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

public class RippleStack extends ListNode<Value> implements RippleValue
{
	private Value first;
	private RippleStack rest;
	
	public Value getFirst()
	{
		return first;
	}
	
	public RippleStack getRest()
	{
		return rest;
	}
	
	public RippleStack( final Value first )
	{
		this.first = first;
		rest = null;
	}
	
	public RippleStack( final Value first, final RippleStack rest )
	{
		this.first = first;
		this.rest = rest;
	}
	
	public RippleStack push( final Value first )
	{
		return new RippleStack( first, this );
	}

	private static RippleStack invert( ListNode<Value> rs )
	{
		ListNode<Value> in = rs;
		RippleStack out = null;

		while ( null != in )
		{
			out = new RippleStack( in.getFirst(), out );
			in = in.getRest();
		}

		return out;
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		boolean padding = Ripple.listPadding();
		ListNode<Value> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.getExpressionOrder() )
			? this : invert( this );

		p.print( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( null != cur )
		{
			if ( isFirst )
				isFirst = false;
			else
				p.print( " " );
		
			p.print( cur.getFirst() );
		
			cur = cur.getRest();
		}
		
		p.print( padding ? " )" : ")" );
	}

	public Value toRdf( ModelConnection mc )
		throws RippleException
{
return null;
}
}

// kate: tab-width 4
