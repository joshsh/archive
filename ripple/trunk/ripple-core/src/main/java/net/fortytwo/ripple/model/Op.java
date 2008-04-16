/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.io.RipplePrintStream;

public class Op implements StackMapping, RippleValue
{
	private final static int ARITY = 1;

	public void applyTo( final StackContext arg,
				final Sink<StackContext, RippleException> sink )
		throws RippleException
	{
		RippleValue v;
	    RippleList stack = arg.getStack();

		v = stack.getFirst();
		final RippleList rest = stack.getRest();
	
		Sink<Operator, RippleException> opSink = new Sink<Operator, RippleException>()
		{
			public void put( final Operator oper )
				throws RippleException
			{
				sink.put( arg.with( rest.push( oper ) ) );
			}
		};

		Operator.createOperator( v, opSink, arg.getModelConnection() );
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
		return ARITY;
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
	
	public boolean isTransparent()
	{
		return true;
	}

    // Op has no inverse
    public StackMapping inverse() throws RippleException
    {
        return new NullStackMapping();
    }
}

// kate: tab-width 4
