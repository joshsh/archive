package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Value;

/**
 *  A ... which reduces arbitrary expressions to a particular normal form.
 */
public abstract class Evaluator
{
// NOTE: this is identical to Function's applyTo() method.
    public abstract void reduce( RippleStack stack,
                                 Sink<RippleStack> sink,
                                 ModelConnection mc )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
