package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public class StringNode extends Ast
{
    private String value;

    public StringNode( final String value )
    {
        this.value = value;
    }

    public Value evaluate( Context context )
        throws WurfelException
    {
        return Context.newStringLiteral( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
