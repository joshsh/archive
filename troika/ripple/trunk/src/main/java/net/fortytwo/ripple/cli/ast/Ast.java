package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public interface Ast
{
	public abstract void evaluate( Sink<RippleValue> sink,
								QueryContext qc,
								ModelConnection mc )
		throws RippleException;

/*
	public RippleValue evaluate( Interpreter itp, ModelConnection mc )
		throws RippleException;
*/
}

// kate: tab-width 4
