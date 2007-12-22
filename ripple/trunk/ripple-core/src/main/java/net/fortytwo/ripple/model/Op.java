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

public class Op implements Function, RippleValue
{
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

		Operator.createOperator( v, opSink, mc );
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
		return 0;
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
}

// kate: tab-width 4
