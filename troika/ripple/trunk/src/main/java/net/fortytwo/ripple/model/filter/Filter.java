package net.fortytwo.ripple.model.filter;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Sink;

// HACK: any child class is a cheat which will not translate properly into RDF.
public abstract class Filter implements Function
{
	public int arity()
	{
		return 1;
	}

public void printTo( RipplePrintStream p )
	throws RippleException
{}

public RdfValue toRdf( ModelConnection mc )
	throws RippleException
{
return null;
}

	public boolean isOperator()
	{
		return false;
	}
}

// kate: tab-width 4
