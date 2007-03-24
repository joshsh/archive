package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

public class StringNode implements Ast
{
    private String value, language;

    public StringNode( final String value )
    {
        this.value = value;
        this.language = null;
    }

    public StringNode( final String value, final String language )
    {
        this.value = value;
        this.language = language;
    }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        return ( null == language )
            ? mc.createValue( value )
            : mc.createValue( value, language );
    }

    public String toString()
    {
        return "\"" + value + "\"" + ( ( null == language ) ? "" : ( "@" + language ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
