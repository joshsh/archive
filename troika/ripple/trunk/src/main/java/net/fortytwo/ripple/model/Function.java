package net.fortytwo.ripple.model;

import wurfel.WurfelException;

import org.openrdf.model.Value;
import org.openrdf.model.URI;

public interface Function extends WurfelValue
{
//    public abstract String toString();
    public URI getUri();

    /**
     *  The fixed number of arguments which this function consumes before
     *  yielding a result.
     */
    public int arity();

// NOTE: this is identical to Evaluator's evaluate() method.
    public void applyTo( ListNode<Value> args,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws WurfelException;

    public void checkArguments( ListNode<Value> args )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
