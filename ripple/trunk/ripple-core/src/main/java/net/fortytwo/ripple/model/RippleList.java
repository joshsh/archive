/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.ListNode;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.vocabulary.RDF;

public class RippleList extends ListNode<RippleValue> implements RippleValue
{
	private static final RdfValue RDF_FIRST = new RdfValue( RDF.FIRST );
	private static final RdfValue RDF_REST = new RdfValue( RDF.REST );
	private static final RdfValue RDF_NIL = new RdfValue( RDF.NIL );

// FIXME: depends on RDF_NIL being defined before the constructor is called.
	public static final RippleList NIL = new RippleList();

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
	
	public RippleValue get( final int i )
		throws RippleException
	{
		if ( i < 0 )
		{
			throw new RippleException( "list index out of bounds: " + i );
		}

		RippleList cur = this;
		for ( int j = 0; j < i; j++ )
		{
			if ( NIL == cur )
			{
				throw new RippleException( "list index out of bounds: " + i );
			}

			cur = cur.getRest();
		}

		return cur.getFirst();
	}

	public int length()
	{
		int l = 0;

		RippleList cur = this;
		while ( NIL != cur )
		{
			l++;
			cur = cur.getRest();
		}

		return l;
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

	public Sink<RippleValue> createPushSink( final Sink<RippleList> sink )
	{
		final RippleList rest = this;

		return new Sink<RippleValue>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( new RippleList( v, rest ) );
			}
		};
	}

	private RippleList()
	{
		// Note: this dummy value avoids null pointer exceptions in the list
		//       memoizer.
		first = new NumericLiteral( 42 );
		
		rest = null;

		rdfEquivalent = RDF_NIL;
	}

	public RippleList push( final RippleValue first )
	{
		return new RippleList( first, this );
	}

	public static RippleList invert( final ListNode<RippleValue> rs )
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

	public RdfValue toRdf( final ModelConnection mc )
		throws RippleException
	{
		if ( null == rdfEquivalent )
		{
net.fortytwo.ripple.io.RdfImporter importer = new net.fortytwo.ripple.io.RdfImporter( mc );
			putRdfStatements( importer.statementSink(), mc );
			
			// This is important, because the caller of toRdf() may expect to
			// read statements immediately.
			mc.commit();
		}

		return rdfEquivalent;
	}

	public void putRdfStatements( final Sink<Statement> sink, final ModelConnection mc )
		throws RippleException
	{
		RippleList cur = this;
		Resource prevRdf = null;

		do
		{
//System.out.println( "cur = " + cur );
			Resource curRdf;

			// Associate list nodes with RDF values.
			if ( null == cur.rdfEquivalent )
			{
				curRdf = mc.createBNode();
				cur.rdfEquivalent = new RdfValue( curRdf );
			}

			// Currently, only RippleList.NIL will already have an RDF equivalent.
			else
			{
				curRdf = (Resource) cur.rdfEquivalent.getRdfValue();
			}
//System.out.println( "    cur.rdfEquivalent = " + cur.rdfEquivalent );

			// Annotate the head of the list with a type, but don't bother
			// annotating every node in the list.
			if ( null == prevRdf )
			{
				if ( RDF.NIL != prevRdf )
				{
//System.out.println( "    putting type statement" );
//					sink.put(
					mc.add(
						mc.createStatement( curRdf, RDF.TYPE, RDF.LIST ) );
				}
			}

			else
			{
//System.out.println( "    putting rest statement" );
//				sink.put(
				mc.add(
					mc.createStatement( prevRdf, RDF.REST, curRdf ) );
			}

			if ( RDF.NIL != curRdf )
			{
//System.out.println( "    putting first statement" );
//				sink.put(
				mc.add(
					mc.createStatement( curRdf, RDF.FIRST, cur.first.toRdf( mc ).getRdfValue() ) );
			}

			prevRdf = curRdf;
			cur = cur.getRest();
		} while ( RDF.NIL != prevRdf );
	}

	public static RippleList concat( final RippleList head, final RippleList tail )
	{
		return ( NIL == head )
			? tail
			: new RippleList( head.first, concat( head.rest, tail ) );
	}

	public static void from( final RippleValue v,
							final Sink<RippleList> sink,
							final ModelConnection mc )
		throws RippleException
	{
		// If already a list...
		if ( v instanceof RippleList )
		{
			sink.put( (RippleList) v );
		}

/* TODO
		// Towards a more general notion of lists...
		else
		{
			Sink<Operator> opSink = new Sink<Operator>()
			{
				public void put( final Operator op )
					throws RippleException
				{
					sink.put( new RippleList( op ) );
				}
			};

			Operator.createOperator( v, opSink, mc );
		}
*/

		// If the argument is an RDF value, try to convert it to a native list.
		else if ( v instanceof RdfValue )
		{
			createList( (RdfValue) v, sink, mc );
		}

		// Otherwise, fail.
		else
		{
			throw new RippleException( "expecting " + RippleList.class + ", found " + v );
		}
	}

// TODO: handle circular lists and other convergent structures 
	private static void createList( final RippleValue head,
									final Sink<RippleList> sink,
									final ModelConnection mc )
		throws RippleException
	{
		if ( head.equals( RDF_NIL ) )
		{
			sink.put( NIL );
		}

		else
		{
			final Collector<RippleValue> firstValues = new Collector<RippleValue>();

			final Sink<RippleList> restSink = new Sink<RippleList>()
			{
				public void put( final RippleList rest ) throws RippleException
				{
					Sink<RippleValue> firstSink = new Sink<RippleValue>()
					{
						public void put( final RippleValue first ) throws RippleException
						{
							RippleList list = new RippleList( first, rest );
							list.rdfEquivalent = head.toRdf( mc );
							sink.put( list );
						}
					};
			
					firstValues.writeTo( firstSink );
				}
			};
			
			Sink<RippleValue> rdfRestSink = new Sink<RippleValue>()
			{
				public void put( final RippleValue rest ) throws RippleException
				{
					// Recurse.
					createList( rest, restSink, mc );
				}
			};
			
			/*
			Sink<RdfValue> rdfFirstSink = new Sink<RdfValue>()
			{
				public void put( final RdfValue first ) throws RippleException
				{
					// Note: it might be more efficient to use ModelBridge only
					//       lazily, binding RDF to generic RippleValues on an
					//       as-needed basis.  However, for now there is no better
					//       place to do this when we're coming from an rdf:List.
					//       Consider a list containing operators.
					firstValues.put( mc.getModel().getBridge().get( first ) );
				}
			};*/

			mc.multiply( head, RDF_FIRST, firstValues );
			mc.multiply( head, RDF_REST, rdfRestSink );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public String toString()
	{
		StringBuilder sb = new StringBuilder();

		boolean padding = Ripple.listPadding();

		ListNode<RippleValue> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.expressionOrder() )
			? this: invert( this );
		RippleValue stateVal = null;

		sb.append( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( null == stateVal )
			{
				if ( Operator.OP == val )
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						sb.append( " " );
					}

					sb.append( val );
				}

				else
				{
					stateVal = val;
				}
			}

			else
			{
				if ( Operator.OP == val )
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						sb.append( " " );
					}

					sb.append( "/" );
					sb.append( stateVal );
					stateVal = null;
				}

				else
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						sb.append( " " );
					}

					sb.append( stateVal );
					stateVal = val;
				}
			}

			cur = cur.getRest();
		}

		if ( null != stateVal )
		{
			if ( !isFirst )
			{
				sb.append( " " );
			}

			sb.append( stateVal );
		}
		
		sb.append( padding ? " )" : ")" );

		return sb.toString();
	}

	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
		boolean padding = Ripple.listPadding();

		ListNode<RippleValue> cur =
			( Ripple.ExpressionOrder.DIAGRAMMATIC == Ripple.expressionOrder() )
			? this : invert( this );
		RippleValue stateVal = null;

		p.print( padding ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( null == stateVal )
			{
				if ( Operator.OP == val )
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						p.print( " " );
					}

					p.print( val );
				}

				else
				{
					stateVal = val;
				}
			}

			else
			{
				if ( Operator.OP == val )
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						p.print( " " );
					}

					p.print( "/" );
					p.print( stateVal );
					stateVal = null;
				}

				else
				{
					if ( isFirst )
					{
						isFirst = false;
					}

					else
					{
						p.print( " " );
					}

					p.print( stateVal );
					stateVal = val;
				}
			}

			cur = cur.getRest();
		}

		if ( null != stateVal )
		{
			if ( !isFirst )
			{
				p.print( " " );
			}

			p.print( stateVal );
		}
		
		p.print( padding ? " )" : ")" );
	}

	public boolean equals( final Object o )
	{
		if ( o instanceof ListNode )
		{
			return equals( (ListNode<RippleValue>) o );
		}

		else
		{
			return false;
		}
	}

	public int compareTo( final RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof RippleList )
		{
			RippleList curThis = this, curOther = (RippleList) other;

			while ( NIL != curThis )
			{
				if ( NIL == curOther )
				{
					return 1;
				}

				int cmp = curThis.first.compareTo( curOther.first );
				if ( 0 != cmp )
				{
					return cmp;
				}

				curThis = curThis.rest;
				curOther = curOther.rest;
			}

			if ( NIL == curOther )
			{
				return 0;
			}

			else
			{
				return -1;
			}
		}

		else
		{
			return RippleList.class.getName().compareTo( other.getClass().getName() );
		}
	}

/*
	public void writeStatementsTo( final Sink<Statement> sink,
									final ModelConnection mc )
		throws RippleException
	{
		RippleList cur = this;
		while ( NIL != cur )
		{
			RippleValue f = cur.getFirst();
			RippleList r = cur.getRest();

			
			Statement stf = mc.createStatement( cur.toRdf( mc ), 
		}
	}
*/

	public void writeStatementsTo( final Sink<Statement> sink,
									final ModelConnection mc )
		throws RippleException
	{
		writeStatementsTo( toRdf( mc ), sink, mc );
	}

	public static void writeStatementsTo( final RippleValue head,
											final Sink<Statement> sink,
											final ModelConnection mc )
		throws RippleException
	{
		if ( 0 == head.compareTo( RDF_NIL ) )
		{
			return;
		}

		if ( !( head.toRdf( mc ).getRdfValue() instanceof Resource ) )
		{
			return;
		}

		final Resource headVal = (Resource) head.toRdf( mc ).getRdfValue();

		Sink<RippleValue> firstSink = new Sink<RippleValue>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( mc.createStatement(
					headVal, RDF.FIRST, v.toRdf( mc ).getRdfValue() ) );
			}
		};

		Sink<RippleValue> restSink = new Sink<RippleValue>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( mc.createStatement(
					headVal, RDF.REST, v.toRdf( mc ).getRdfValue() ) );

				// Recurse.
				writeStatementsTo( v, sink, mc );
			}
		};

		// Annotate the head of the list with a type, but don't bother
		// annotating every node in the list.
		sink.put( mc.createStatement(
			headVal, RDF.TYPE, RDF.LIST ) );

		mc.multiply( head, RDF_FIRST, firstSink );
		mc.multiply( head, RDF_REST, restSink );
	}
}

// kate: tab-width 4
