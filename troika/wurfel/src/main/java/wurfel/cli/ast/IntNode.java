package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public class IntNode extends Ast
{
    private int value;

    public IntNode( final int value )
    {
        this.value = value;
    }

    public Value evaluate( Context context )
        throws WurfelException
    {
        return context.createLiteral( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
