package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.ListNode;

import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;

public class RippleList extends ListNode<RippleValue> implements RippleValue
{
	private RippleValue first;
	private RippleList rest;

	private RdfValue rdfEquivalent = null;

	public RippleValue getFirst()
	{
		return first;
	}

	public RippleList getRest()
	{
		return rest;
	}
	
	public RippleList( final RippleValue first )
	{
		this.first = first;
		rest = null;
	}
	
	public RippleList( final RippleValue first, final RippleList rest )
	{
		this.first = first;
		this.rest = rest;
	}

/*
	public static RippleList fromRdf( RdfValue v, ModelConnection mc )
	{
		
	}
*/

	// Note: this constructor does not allow you to create a null list from
	//       rdf:nil.
	public RippleList( RdfValue v, ModelConnection mc )
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

			// Note: it might be more efficient to use ModelBridge only
			//       lazily, binding RDF to generic RippleValues on an
			//       as-needed basis.  However, for now there is no better
			//       place to do this when we're coming from an rdf:List.
			//       Consider a list containing operators.
			first = mc.getModel().getBridge().get(
				mc.findUniqueProduct( curRdf, rdfFirst ) );
System.out.println( "first = " + first );

			curRdf = mc.findUniqueProduct( curRdf, rdfRest );
			if ( curRdf.equals( rdfNil ) )
				break;

			else
			{
				rest = new RippleList( first, rest );
				rest.rdfEquivalent = rdfEquivalent;
			}
		}
System.out.println( "resulting list: " + toString() );
	}

	public RippleList push( final RippleValue first )
	{
		return new RippleList( first, this );
	}

	public static RippleList invert( ListNode<RippleValue> rs )
	{
		ListNode<RippleValue> in = rs;
		RippleList out = null;

		while ( null != in )
		{
			out = new RippleList( in.getFirst(), out );
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

			RippleList cur = invert( this );

			while ( cur != null )
			{
				if ( cur.first.isOperator() )
				{
					System.out.println( "it's an operator!" );
					mc.add( curRdf, rdfFirst, ( (Operator) cur.first ).getFunction().toRdf( mc ) );
					RdfValue restRdf = new RdfValue( mc.createBNode() );
					mc.add( curRdf, rdfRest, restRdf );
					curRdf = restRdf;

					// hack...
					mc.add( curRdf, rdfFirst, ((Operator) cur.first).toRdf( mc ) );
				}
				else
					mc.add( curRdf, rdfFirst, cur.first.toRdf( mc ) );
				RippleList rest = cur.rest;
				RdfValue restRdf = ( null == cur.rest )
					? rdfNil : new RdfValue( mc.createBNode() );
				mc.add( curRdf, rdfRest, restRdf );
				curRdf = restRdf;
				cur = cur.rest;
			}
		}

		return rdfEquivalent;
	}

	public static RippleList concat( RippleList head, RippleList tail )
	{
		return ( null == head )
			? tail
			: new RippleList( head.first, concat( head.rest, tail ) );
	}

	public static RippleList from( RippleValue v, ModelConnection mc )
		throws RippleException
	{
		if ( v instanceof RippleList )
			return (RippleList) v;

		// If not (already) a list...
		else
		{
			// If the argument is an RDF value, try to convert it to a native list.
			if ( v instanceof RdfValue )
				return new RippleList( (RdfValue) v, mc );

			// Otherwise, fail.
			else
				throw new RippleException( "expecting " + RippleList.class + ", found " + v );
		}
	}
}

// kate: tab-width 4
