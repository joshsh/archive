package net.fortytwo.ripple.model;

import wurfel.WurfelException;

import org.openrdf.model.Value;

/**
 *  A ... which reduces arbitrary expressions to a particular normal form.
 */
public abstract class Evaluator
{
    protected static boolean isApply( Value v )
    {
        return v instanceof Apply;
    }

// NOTE: this is identical to Function's applyTo() method.
    public abstract void reduce( ListNode<Value> expr,
                                 Sink<ListNode<Value>> sink,
                                 ModelConnection mc )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
