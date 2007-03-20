package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.ListNode;

import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;

public class RippleStack extends ListNode<RippleValue> implements RippleValue
{
	private RippleValue first;
	private RippleStack rest;

	private RdfValue rdfEquivalent = null;

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

	public RippleStack( RdfValue v, ModelConnection mc )
		throws RippleException
	{
//		if ( v.equals( rdfNil ) )
//			return null;

		first = mc.findUniqueProduct( v, rdfFirst );
		rest = new RippleStack(
			mc.findUniqueProduct( v, rdfRest ), mc );

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

		ListNode<RippleValue> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.getExpressionOrder() )
			? invert( this ) : this;

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

	public boolean isOperator()
	{
		return false;
	}

	private static RdfValue rdfFirst = new RdfValue( RDF.FIRST );
	private static RdfValue rdfRest = new RdfValue( RDF.REST );
	private static RdfValue rdfNil = new RdfValue( RDF.NIL );

	public RdfValue toRdf( ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
			rdfEquivalent = new RdfValue( mc.createBNode() );

			mc.add( rdfEquivalent, rdfFirst, first.toRdf( mc ) );
			if ( null == rest )
				mc.add( rdfEquivalent, rdfRest, rdfNil );
			else
				mc.add( rdfEquivalent, rdfRest, rest.toRdf( mc ) );
		}

		return rdfEquivalent;
	}
}

// kate: tab-width 4
