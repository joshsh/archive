package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public interface Ast
{
    public abstract void evaluate( Sink<RippleValue> sink,
                                   Interpreter itp,
                                   ModelConnection mc )
        throws RippleException;

/*
    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException;
*/
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
