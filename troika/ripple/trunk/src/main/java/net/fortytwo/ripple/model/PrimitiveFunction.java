package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public abstract class PrimitiveFunction implements Function
{
	private RdfValue rdfEquivalent;
	private FunctionTypeAnnotation typeAnnotation;

	public PrimitiveFunction( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		rdfEquivalent = v;

//		typeAnnotation = new FunctionTypeAnnotation( v, mc );
typeAnnotation = null;
	}

	public void printTo( RipplePrintStream p )
		throws RippleException
	{
		p.print( rdfEquivalent );
	}

    public RdfValue toRdf( ModelConnection mc )
        throws RippleException
    {
		return rdfEquivalent;
    }

	public String toString()
    {
		return rdfEquivalent.toString();
	}

	public boolean isFunctionEnvelope()
	{
		return false;
	}
}

// kate: tab-width 4
