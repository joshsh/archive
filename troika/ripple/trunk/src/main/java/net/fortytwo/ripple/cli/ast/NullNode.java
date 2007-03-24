package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

/**
 *  Warning: use this class with caution.
 */
public class NullNode implements Ast
{
    public NullNode()
    { }

    public void evaluate( Sink<RippleValue> sink,
                          Interpreter itp,
                          ModelConnection mc )
        throws RippleException
    {}

    public String toString()
    {
        return "[null]";
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
