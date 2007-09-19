/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.vocabulary.RDF;

public class Op implements Function, RippleValue
{
	private static final RdfValue RDF_FIRST = new RdfValue( RDF.FIRST );
	private static final RdfValue RDF_NIL = new RdfValue( RDF.NIL );

	public void applyTo( RippleList stack,
				final Sink<RippleList> sink,
				final ModelConnection mc )
		throws RippleException
	{
		RippleValue v;
	
		v = stack.getFirst();
		final RippleList rest = stack.getRest();
	
		Sink<Operator> opSink = new Sink<Operator>()
		{
			public void put( final Operator oper )
				throws RippleException
			{
				sink.put( new RippleList( oper, rest ) );
			}
		};

		createOperator( v, opSink, mc );
	}

	public int compareTo( final RippleValue other )
	{
		// There is only one Op.
		if ( other == this )
		{
			return 0;
		}

		else
		{
			return this.getClass().getName().compareTo( other.getClass().getName() );
		}
	}

	public int arity()
	{
		return 1;
	}

	public void printTo( final RipplePrintStream p )
		throws RippleException
	{
		System.err.println( "You should not need to print op directly." );
		System.exit( 1 );
	}

	public RdfValue toRdf( final ModelConnection mc )
		throws RippleException
	{
		System.err.println( "You should not need to convert op explicitly." );
		System.exit( 1 );
		return null;
	}

	public boolean isActive()
	{
		System.err.println( "op is always active, but your algorithm should know that without calling isActive()." );
		System.exit( 1 );
		return true;
	}

	/**
	 *  Finds the type of a value and creates an appropriate "active" wrapper.
	 */
	private void createOperator( final RippleValue v,
								final Sink<Operator> opSink,
								final ModelConnection mc )
		throws RippleException
	{
		// A function becomes active.
		if ( v instanceof Function )
		{
			opSink.put( new Operator( (Function) v ) );
			return;
		}

		// A list is dequoted.
		else if ( v instanceof RippleList )
		{
			opSink.put( new Operator( (RippleList) v ) );
			return;
		}

		// This is the messy part.  Attempt to guess the type of the object from
		// the available RDF statements, and create the appropriate object.
		if ( v instanceof RdfValue )
		{
			if ( isRdfList( (RdfValue) v, mc ) )
			{
				Sink<RippleList> listSink = new Sink<RippleList>()
				{
					public void put( final RippleList list )
						throws RippleException
					{
						opSink.put( new Operator( list ) );
					}
				};

				RippleList.from( v, listSink, mc );
				return;
			}

			// An RDF value not otherwise recognizable becomes a predicate filter.
			else
			{
				opSink.put( new Operator( (RdfValue) v ) );
				return;
			}
		}

		// Anything else becomes an active nullary filter with no output.
		else
		{
			opSink.put( new Operator( new NullFilter() ) );
			return;
		}
	}

// TODO: replace this with something a little more clever
	private boolean isRdfList( final RdfValue v, final ModelConnection mc )
		throws RippleException
	{
		return ( v.equals( RDF_NIL )
			|| null != mc.findSingleObject( (RdfValue) v, RDF_FIRST ) );
	}
}

// kate: tab-width 4
