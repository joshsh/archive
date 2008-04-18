/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model.impl.sesame;

import java.util.HashMap;
import java.util.Map;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.RippleProperties;
import net.fortytwo.ripple.model.enums.ExpressionOrder;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.io.RDFImporter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.flow.Source;
import net.fortytwo.ripple.ListNode;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;

public class SesameList extends RippleList
{
	private static final RdfValue RDF_FIRST = new RdfValue( RDF.FIRST );
	private static final RdfValue RDF_REST = new RdfValue( RDF.REST );
	private static final RdfValue RDF_NIL = new RdfValue( RDF.NIL );

    private static final RippleList NIL = new SesameList();

    private static Map<Value, Source<RippleList, RippleException>> nativeLists = new HashMap<Value, Source<RippleList, RippleException>>();

    private static ExpressionOrder expressionOrder;
    private static boolean printPadded;
    private static boolean initialized = false;

    private Resource rdfEquivalent = null;

    public static RippleList nilList()
    {
        return NIL;
    }

    private static void initialize() throws RippleException
    {
        RippleProperties props = Ripple.getProperties();
        expressionOrder = ExpressionOrder.find(props.getString( Ripple.EXPRESSION_ORDER ) );
        printPadded = props.getBoolean( Ripple.LIST_PADDING );
        initialized = true;
    }
    public SesameList( final RippleValue first )
	{
		this.first = first;
		rest = NIL;
	}
	
	public SesameList( final RippleValue first, final RippleList rest )
	{
		this.first = first;
		this.rest = rest;
	}

    // FIXME: this is temporary
	private SesameList()
	{
		// Note: this is a trick to avoid null pointer exceptions in the list
		// memoizer.
		first = this;
		
		// This should never be dereferenced.
		rest = null;

		// FIXME: depends on RDF_NIL being defined before this constructor is called.
		rdfEquivalent = RDF.NIL;
	}

    public boolean isNil()
    {
        return ( null == rest );
    }

    public RippleList push( final RippleValue first ) throws RippleException
	{
		return new SesameList( first, this );
	}

	public static RippleList invert( final ListNode<RippleValue> rs )
	{
		ListNode<RippleValue> in = rs;
		RippleList out = NIL;

		while ( NIL != in )
		{
			out = new SesameList( in.getFirst(), out );
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
RDFImporter importer = new RDFImporter( mc );
			putRdfStatements( importer.statementSink(), mc );
			
			// This is important, because the caller of toRdf() may expect to
			// read statements immediately.
			mc.commit();
		}

		return new RdfValue( rdfEquivalent );
	}

	private void putRdfStatements( final Sink<Statement, RippleException> sink, final ModelConnection mc )
		throws RippleException
	{
		SesameList cur = this;
		Resource prevRdf = null;

		do
		{
//System.out.println( "cur = " + cur );
			Resource curRdf;
//System.out.println( "    cur.rdfEquivalent (before) = " + cur.rdfEquivalent );

			// Associate list nodes with RDF values.
			if ( null == cur.rdfEquivalent )
			{
				curRdf = mc.createBNode();
				cur.rdfEquivalent = curRdf;
			}

			// Currently, only NIL will already have an RDF equivalent.
			else
			{
				curRdf = cur.rdfEquivalent;
			}
//System.out.println( "    cur.rdfEquivalent = " + cur.rdfEquivalent );

			if ( null == prevRdf )
			{
				// Annotate the head of the list with a type, but don't bother
				// annotating every node in the list.
				if ( RDF.NIL != curRdf )
				{
//System.out.println( "    putting type statement" );
					sink.put(
						mc.createStatement( curRdf, RDF.TYPE, RDF.LIST ) );
				}
			}

			else
			{
//System.out.println( "    putting rest statement" );
				sink.put(
					mc.createStatement( prevRdf, RDF.REST, curRdf ) );
			}

			if ( RDF.NIL != curRdf )
			{
//System.out.println( "    putting first statement" );
				sink.put(
					mc.createStatement( curRdf, RDF.FIRST, cur.first.toRdf( mc ).getRdfValue() ) );
			}

			prevRdf = curRdf;
			cur = (SesameList) cur.getRest();
		} while ( RDF.NIL != prevRdf );
	}

	public static RippleList concat( final RippleList head, final RippleList tail )
	{
		return ( NIL == head )
			? tail
			: new SesameList( head.getFirst(), concat( head.getRest(), tail ) );
	}

	public static void from( final RippleValue v,
							final Sink<RippleList, RippleException> sink,
							final ModelConnection mc )
		throws RippleException
	{
        boolean memoize = Ripple.getProperties().getBoolean( Ripple.MEMOIZE_LISTS_FROM_RDF );

        // If already a list...
		if ( v instanceof RippleList )
		{
			sink.put( (RippleList) v );
		}

		// If the argument is an RDF value, try to convert it to a native list.
		else if ( v instanceof RdfValue )
		{
			if ( memoize )
			{
//System.out.println("looking for source for list: " + v);
				Value rdfVal = ( (RdfValue) v ).toRdf( mc ).getRdfValue();
				Source<RippleList, RippleException> source = nativeLists.get( rdfVal );
				if ( null == source )
				{
					Collector<RippleList, RippleException> coll = new Collector<RippleList, RippleException>();
					
					createList( (RdfValue) v, coll, mc );
					
					source = coll;
					nativeLists.put( rdfVal, source );
				}
//else System.out.println("   found source for list");
				
				source.writeTo( sink );
			}
			
			else
			{
				createList( (RdfValue) v, sink, mc );
			}
		}

		// Towards a more general notion of lists
		else
		{
			createConceptualList( v, sink, mc );
		}

		/*
		// Otherwise, fail.
		else
		{
			throw new RippleException( "expecting " + RippleList.class + ", found " + v );
		}*/
	}

// TODO: find a better name
	private static void createConceptualList( final RippleValue head,
											final Sink<RippleList, RippleException> sink,
											final ModelConnection mc )
		throws RippleException
	{
		/*
		Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator op )
				throws RippleException
			{
				sink.put( mc.list( op ) );
			}
		};

		Operator.createOperator( head, opSink, mc );
		*/
		
		sink.put( new SesameList( Operator.OP ).push( head ) );
	}
	
// TODO: extend circular lists and other convergent structures
	private static void createList( final RippleValue head,
									final Sink<RippleList, RippleException> sink,
									final ModelConnection mc )
		throws RippleException
	{	
		if ( head.equals( RDF_NIL ) )
		{
			sink.put( NIL );
		}

		else
		{
			final Collector<RippleValue, RippleException> firstValues = new Collector<RippleValue, RippleException>();

			final Sink<RippleList, RippleException> restSink = new Sink<RippleList, RippleException>()
			{
				public void put( final RippleList rest ) throws RippleException
				{
					Sink<RippleValue, RippleException> firstSink = new Sink<RippleValue, RippleException>()
					{
						public void put( final RippleValue first ) throws RippleException
						{
							SesameList list = new SesameList( first, rest );
							list.rdfEquivalent = (Resource) head.toRdf( mc ).getRdfValue();
							sink.put( list );
						}
					};
			
					firstValues.writeTo( firstSink );
				}
			};
			
			Sink<RippleValue, RippleException> rdfRestSink = new Sink<RippleValue, RippleException>()
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

			mc.multiply( head, RDF_FIRST, firstValues, false );
			
			if ( firstValues.size() > 0 || head.toRdf( mc ).getRdfValue().equals( RDF.NIL ) )
			{
				mc.multiply( head, RDF_REST, rdfRestSink, false );
			}
			
			else
			{
				createConceptualList( head, sink, mc );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public String toString()
	{
        if ( !initialized )
        {
            try {
                initialize();
            } catch (RippleException e) {
                initialized = true;
                e.logError();
            }
        }

        StringBuilder sb = new StringBuilder();

		ListNode<RippleValue> cur =
			( ExpressionOrder.DIAGRAMMATIC == expressionOrder )
			? this: invert( this );

		sb.append( printPadded ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( isFirst )
			{
				isFirst = false;
			}
			
			//else if ( Operator.OP != val )
			//{
				sb.append( " " );
			//}

			if ( Operator.OP == val )
			{
				sb.append( "!" );
			}

			else
			{
				sb.append( val );
			}

			cur = cur.getRest();
		}
		
		sb.append( printPadded ? " )" : ")" );

		return sb.toString();
	}

	// Note: assumes diagrammatic order
	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
        if ( !initialized )
        {
            initialize();
        }

		ListNode<RippleValue> cur =
			( ExpressionOrder.DIAGRAMMATIC == expressionOrder )
			? this : invert( this );

		p.print( printPadded ? "( " : "(" );
		
		boolean isFirst = true;
		while ( NIL != cur )
		{
			RippleValue val = cur.getFirst();

			if ( isFirst )
			{
				isFirst = false;
			}

			else if ( Operator.OP != val )
			{
				p.print( " " );
			}

			if ( Operator.OP == val )
			{
				p.print( "!" );
			}

			else
			{
				p.print( val );
			}
			
			cur = cur.getRest();
		}

		p.print( printPadded ? " )" : ")" );
	}

	public boolean equals( final Object o )
	{
		if ( o instanceof ListNode )
		{
			if ( o == NIL && this == NIL )
			{
				return true;
			}
			
			else
			{
				return equals( (ListNode<RippleValue>) o );
			}
		}

		else
		{
			return false;
		}
	}

	public int compareTo( final RippleValue other )
	{
//System.out.println( "[" + this + "].compareTo(" + other + ")" );
		if ( other instanceof SesameList )
		{
			SesameList curThis = this, curOther = (SesameList) other;

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

				curThis = (SesameList) curThis.getRest();
				curOther = (SesameList) curOther.getRest();
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

	public void writeStatementsTo( final Sink<Statement, RippleException> sink,
									final ModelConnection mc )
		throws RippleException
	{
		writeStatementsTo( toRdf( mc ), sink, mc );
	}

	public static void writeStatementsTo( final RippleValue head,
											final Sink<Statement, RippleException> sink,
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

		Sink<RippleValue, RippleException> firstSink = new Sink<RippleValue, RippleException>()
		{
			public void put( final RippleValue v ) throws RippleException
			{
				sink.put( mc.createStatement(
					headVal, RDF.FIRST, v.toRdf( mc ).getRdfValue() ) );
			}
		};

		Sink<RippleValue, RippleException> restSink = new Sink<RippleValue, RippleException>()
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

		mc.multiply( head, RDF_FIRST, firstSink, false );
		mc.multiply( head, RDF_REST, restSink, false );
	}
}
