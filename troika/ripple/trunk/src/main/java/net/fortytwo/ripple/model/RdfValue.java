package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

public class RdfValue implements RippleValue
{
	private Value value;
	
	public RdfValue( Value value )
	{
		this.value = value;
	}

	public Value toRdf( ModelConnection mc )
	{
		return value;
	}

	public RippleValue toNative()
	{
return null;
	}

    public void printTo( RipplePrintStream p )
        throws RippleException
{
}


/*
	private static Literal toLiteral( Value v )
        throws RippleException
    {
        if ( v instanceof Literal )
            return (Literal) v;

        else
            throw new RippleException( "value " + v.toString() + " is not a Literal" );
	}

	public static int toInt( RippleValue v, ModelConnection mc )
	{
		RdfValue rdfValue = v.toRdf( mc );
		Literal l = toLiteral( rdfValue.value );

        String label = l.getLabel();
        try
        {
            return ( new Integer( label ) ).intValue();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
	}
*/

}

// kate: tab-width 4
