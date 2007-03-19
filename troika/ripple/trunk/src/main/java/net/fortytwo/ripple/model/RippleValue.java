package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public interface RippleValue
{
    public RdfValue toRdf( ModelConnection mc )
        throws RippleException;

    public boolean isOperator();

    public void printTo( RipplePrintStream p )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
