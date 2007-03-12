package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

/**
 *  Warning: use this class with caution.
 */
public class NullNode extends Ast
{
    public NullNode()
    { }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
