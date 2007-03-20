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
        // An Evaluator performs reduction; it does not need the stack to be
        // reduced to any level before it is received as input.
        return 0;
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

    public RdfValue toRdf( ModelConnection mc )
        throws RippleException
    {
if ( null == rdfEquivalent )
    rdfEquivalent = new RdfValue( mc.createBNode() );

return rdfEquivalent;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
