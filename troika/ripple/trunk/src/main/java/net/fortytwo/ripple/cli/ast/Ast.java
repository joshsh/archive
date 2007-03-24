package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

public interface Ast
{
/*
    public abstract RippleValue evaluate( RippleList stack,
                                          Sink<RippleList> sink,
                                          Interpreter itp,
                                          ModelConnection mc )
        throws RippleException;
*/

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
