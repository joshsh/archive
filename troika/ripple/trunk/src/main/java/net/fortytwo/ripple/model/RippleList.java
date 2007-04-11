package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.ListNode;

import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;

public class RippleList extends ListNode<RippleValue> implements RippleValue
{
	public static RippleList NIL
		= new RippleList( (RippleValue) null, (RippleList) null );

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
		rest = NIL;
	}
	
	public RippleList( final RippleValue first, final RippleList rest )
	{
		this.first = first;
		this.rest = rest;
	}

	public static RippleList createList( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		if ( v.equals( rdfNil ) )
			return NIL;

		else
			return new RippleList( v, mc );
	}

	private RippleList( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		RdfValue curRdf = v;
		rest = NIL;

		for (;;)  // break out when we get to rdf:nil
		{
			rdfEquivalent = curRdf;

			// Note: it might be more efficient to use ModelBridge only
			//       lazily, binding RDF to generic RippleValues on an
			//       as-needed basis.  However, for now there is no better
			//       place to do this when we're coming from an rdf:List.
			//       Consider a list containing operators.
			first = mc.getModel().getBridge().get(
				mc.findUniqueProduct( curRdf, rdfFirst ) );

			curRdf = mc.findUniqueProduct( curRdf, rdfRest );
			if ( curRdf.equals( rdfNil ) )
				break;

			else
			{
				rest = new RippleList( first, rest );
				rest.rdfEquivalent = rdfEquivalent;
			}
		}
	}

	public RippleList push( final RippleValue first )
	{
		return new RippleList( first, this );
	}

	public static RippleList invert( ListNode<RippleValue> rs )
	{
		ListNode<RippleValue> in = rs;
		RippleList out = NIL;

		while ( NIL != in )
		{
			out = new RippleList( in.getFirst(), out );
			in = in.getRest();
		}

		return out;
	}

	public boolean isActive()
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
			if ( NIL == this )
				rdfEquivalent = rdfNil;

			else
			{
				rdfEquivalent = new RdfValue( mc.createBNode() );
				RdfValue curRdf = rdfEquivalent;
	
				// Annotate the head of the list with a type, but don't bother
				// annotating every node in the list.
				mc.add( curRdf, rdfType, rdfList );
	
				RippleList cur = invert( this );
	
				while ( cur != NIL )
				{
					mc.add( curRdf, rdfFirst, cur.first.toRdf( mc ) );
					RippleList rest = cur.rest;
					RdfValue restRdf = ( NIL == cur.rest )
						? rdfNil : new RdfValue( mc.createBNode() );
					mc.add( curRdf, rdfRest, restRdf );
					curRdf = restRdf;
					cur = cur.rest;
				}
			}
		}

		return rdfEquivalent;
	}

	public static RippleList concat( RippleList head, RippleList tail )
	{
		return ( NIL == head )
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
				return createList( (RdfValue) v, mc );

			// Otherwise, fail.
			else
				throw new RippleException( "expecting " + RippleList.class + ", found " + v );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		boolean padding = Ripple.listPadding();

		ListNode<RippleValue> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.getExpressionOrder() )
			? invert( this ) : this;
		RippleValue stateVal = null;

		sb.append( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( null == stateVal )
			{
				if ( val.equals( Operator.OP ) )
				{
					if ( isFirst )
						isFirst = false;
					else
						sb.append( " " );

					sb.append( val );
				}

				else
					stateVal = val;
			}

			else
			{
				if ( val.equals( Operator.OP ) )
				{
					if ( isFirst )
						isFirst = false;
					else
						sb.append( " " );

					sb.append( "/" );
					sb.append( stateVal );
					stateVal = null;
				}

				else
				{
					if ( isFirst )
						isFirst = false;
					else
						sb.append( " " );

					sb.append( stateVal );
					stateVal = val;
				}
			}

			cur = cur.getRest();
		}

		if ( null != stateVal )
		{
			if ( !isFirst )
				sb.append( " " );

			sb.append( stateVal );
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
		RippleValue stateVal = null;

		p.print( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( null == stateVal )
			{
				if ( val.equals( Operator.OP ) )
				{
					if ( isFirst )
						isFirst = false;
					else
						p.print( " " );

					p.print( val );
				}

				else
					stateVal = val;
			}

			else
			{
				if ( val.equals( Operator.OP ) )
				{
					if ( isFirst )
						isFirst = false;
					else
						p.print( " " );

					p.print( "/" );
					p.print( stateVal );
					stateVal = null;
				}

				else
				{
					if ( isFirst )
						isFirst = false;
					else
						p.print( " " );

					p.print( stateVal );
					stateVal = val;
				}
			}

			cur = cur.getRest();
		}

		if ( null != stateVal )
		{
			if ( !isFirst )
				p.print( " " );

			p.print( stateVal );
		}
		
		p.print( padding ? " )" : ")" );
	}
}

// kate: tab-width 4
