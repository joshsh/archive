package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;

public class ApplyNode extends Ast
{
    private Ast arg;

    public ApplyNode( final Ast argument )
    {
        arg = argument;
    }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        RippleValue v = arg.evaluate( itp, mc );

        return Operator.createOperator( v, mc );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
