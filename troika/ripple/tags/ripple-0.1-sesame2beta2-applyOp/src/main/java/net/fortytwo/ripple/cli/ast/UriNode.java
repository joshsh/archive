package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public class UriNode extends Ast
{
    private String value;

    public UriNode( final String value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return mc.createUri( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
