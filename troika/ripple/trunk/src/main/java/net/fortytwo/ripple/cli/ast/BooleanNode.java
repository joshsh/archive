package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class BooleanNode implements Ast
{
    private boolean value;

    public BooleanNode( final boolean value )
    {
        this.value = value;
    }

    public void evaluate( Sink<RippleValue> sink,
                          Interpreter itp,
                          ModelConnection mc )
        throws RippleException
    {
        sink.put( mc.createValue( value ) );
    }

    public String toString()
    {
        return "" + value;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
