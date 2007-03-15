package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

/**
 *  A marker interface indicating a value with custom behavior.
 */
public interface RippleValue extends Value
{
    public void printTo( RipplePrintStream p )
        throws RippleException;

    public Value toRdf( ModelConnection mc )
        throws RippleException;

    public boolean isFunctionEnvelope();

    /**
     *  @return  the Value
    public Value proxy();
*/
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
