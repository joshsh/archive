package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

/**
 *  Warning: use this class with caution.
 */
public class NullNode implements Ast
{
    public NullNode()
    { }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return null;
    }

    public String toString()
    {
        return "[null]";
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
