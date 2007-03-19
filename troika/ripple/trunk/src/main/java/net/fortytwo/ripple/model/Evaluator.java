package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

/**
 *  A function which reduces arbitrary expressions to a particular normal form.
 */
public abstract class Evaluator implements Function
{
    private RdfValue rdfEquivalent = null;

    public int arity()
    {
        return 1;
    }

    public boolean isOperator()
    {
        return false;
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
p.print( "[Evaluator]" );
    }

    public RippleValue toRdf( ModelConnection mc )
        throws RippleException
    {
if ( null == rdfEquivalent )
    rdfEquivalent = new RdfValue( mc.createBNode() );

return rdfEquivalent;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
