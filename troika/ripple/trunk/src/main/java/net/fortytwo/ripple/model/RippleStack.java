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

/*
	public static RippleStack fromRdf( RdfValue v, ModelConnection mc )
	{
		
	}
*/

	// Note: this constructor does not allow you to create a null list from
	//       rdf:nil.
	public RippleStack( RdfValue v, ModelConnection mc )
		throws RippleException
	{
//		if ( v.equals( rdfNil ) )
//			return null;
		RdfValue curRdf = v;
		rest = null;

		for (;;)  // break out when we get to rdf:nil
		{
			rdfEquivalent = curRdf;
System.out.println( "rdfEquivalent = " + rdfEquivalent );

			// lazy initialization (we point first at an RdfValue directly,
			// rather than using ModelBridge)
			first = mc.findUniqueProduct( curRdf, rdfFirst );
System.out.println( "first = " + first );

			curRdf = mc.findUniqueProduct( curRdf, rdfRest );
			if ( curRdf.equals( rdfNil ) )
				break;

			else
			{
				rest = new RippleStack( first, rest );
				rest.rdfEquivalent = rdfEquivalent;
			}
		}
System.out.println( "resulting list: " + toString() );
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

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		boolean padding = Ripple.listPadding();

		ListNode<RippleValue> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.getExpressionOrder() )
			? invert( this ) : this;

		sb.append( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( null != cur )
		{
			if ( isFirst )
				isFirst = false;
			else
				sb.append( " " );
		
			sb.append( cur.getFirst().toString() );
		
			cur = cur.getRest();
		}
		
		sb.append( padding ? " )" : ")" );

		return sb.toString();
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

	private static RdfValue rdfType = new RdfValue( RDF.TYPE );
	private static RdfValue rdfList = new RdfValue( RDF.LIST );
	private static RdfValue rdfFirst = new RdfValue( RDF.FIRST );
	private static RdfValue rdfRest = new RdfValue( RDF.REST );
	private static RdfValue rdfNil = new RdfValue( RDF.NIL );

	public RdfValue toRdf( ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
			rdfEquivalent = new RdfValue( mc.createBNode() );
			RdfValue curRdf = rdfEquivalent;

			// Annotate the head of the list with a type, but don't bother
			// annotating every node in the list.
			mc.add( curRdf, rdfType, rdfList );

			RippleStack cur = invert( this );

			while ( cur != null )
			{
				mc.add( curRdf, rdfFirst, cur.first.toRdf( mc ) );
				RippleStack rest = cur.rest;
				RdfValue restRdf = ( null == cur.rest )
					? rdfNil : new RdfValue( mc.createBNode() );
				mc.add( curRdf, rdfRest, restRdf );
				curRdf = restRdf;
				cur = cur.rest;
			}
		}

		return rdfEquivalent;
	}
}

// kate: tab-width 4
