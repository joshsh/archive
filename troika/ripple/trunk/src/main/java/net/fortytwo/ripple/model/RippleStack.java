package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.ListNode;

import org.openrdf.model.Value;

public class RippleStack extends ListNode<RippleValue> implements RippleValue
{
	private RippleValue first;
	private RippleStack rest;

	private Value rdfEquivalent = null;

	public RippleValue getFirst()
	{
		return first;
	}

	public RippleStack getRest()
	{
		return rest;
	}
	
	public RippleStack( final RippleValue first )
	{
		this.first = first;
		rest = null;
	}
	
	public RippleStack( final RippleValue first, final RippleStack rest )
	{
		this.first = first;
		this.rest = rest;
	}

	public RippleStack RippleStack( RippleValue v, ModelConnection mc )
		throws RippleException
	{
//		if ( v.equals( RDF.NIL ) )
//			return null;

		first = mc.findUniqueProduct( v, RDF.FIRST );
		rest = new RippleStack(
			mc.findUniqueProduct( v, RDF.REST ).toRdf(), mc );

		rdfEquivalent = v;
	}

	public RippleStack push( final RippleValue first )
	{
		return new RippleStack( first, this );
	}

	private static RippleStack invert( ListNode<RippleValue> rs )
	{
		ListNode<RippleValue> in = rs;
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

	public boolean isFunctionEnvelope()
	{
		return false;
	}

	public Value toRdf( ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
			rdfEquivalent = mc.createBNode();

			mc.add( rdfEquivalent, RDF.FIRST, first.toRdf() );
			if ( null == rest )
				mc.add( rdfEquivalent, RDF.REST, RDF.NIL );
			else
				mc.add( rdfEquivalent, RDF.REST, rest.toRdf() );
		}

		return rdfEquivalent;
	}
}

// kate: tab-width 4
