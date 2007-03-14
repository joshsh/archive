package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;
import org.openrdf.model.URI;

public interface Function extends RippleValue
{
//    public abstract String toString();
    public URI getUri();

    /**
     *  The fixed number of arguments which this function consumes before
     *  yielding a result.
     */
    public int arity();

// NOTE: this is identical to Evaluator's evaluate() method.
    public void applyTo( RippleStack args,
                         Sink<RippleStack> sink,
                         ModelConnection mc )
        throws RippleException;

    public void checkArguments( RippleStack args )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
