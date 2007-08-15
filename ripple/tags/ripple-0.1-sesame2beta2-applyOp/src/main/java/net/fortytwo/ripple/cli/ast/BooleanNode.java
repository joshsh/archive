package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public class BooleanNode extends Ast
{
    private boolean value;

    public BooleanNode( final boolean value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return mc.createLiteral( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
