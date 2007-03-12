package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;

import org.openrdf.model.Value;

public class OperatorNode extends Ast
{
    private Operator value;

    public OperatorNode( Operator op )
    {
        value = op;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        switch ( value )
        {
            case APPLY:
                return mc.getApplyOp();
            default:
                return null;
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
