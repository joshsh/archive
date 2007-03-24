package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

public class IntNode implements Ast
{
    private int value;

    public IntNode( final int value )
    {
        this.value = value;
    }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return mc.createValue( value );
    }

    public String toString()
    {
        return "" + value;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
