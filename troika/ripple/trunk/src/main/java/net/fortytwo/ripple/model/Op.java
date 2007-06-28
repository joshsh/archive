package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RipplePrintStream;
import net.fortytwo.ripple.util.Sink;

public class Op implements Function, RippleValue
{
	public void applyTo( RippleList stack,
				Sink<RippleList> sink,
				ModelConnection mc )
		throws RippleException
	{
		RippleValue v;
	
		v = stack.getFirst();
		stack = stack.getRest();
	
		sink.put( new RippleList(
			Operator.createOperator( v, mc ), stack ) );
	}

	public int compareTo( RippleValue other )
	{
		// There is only one Op.
		if ( other == this )
			return 0;
		else
			return this.getClass().getName().compareTo( other.getClass().getName() );
	}

	public int arity()
	{
		return 1;
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		System.err.println( "You should not need to print op directly." );
		System.exit( 1 );
	}

	public RdfValue toRdf( ModelConnection mc )
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
