package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;

public class IntNode extends Ast
{
    private int value;

    public IntNode( final int value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        return mc.createLiteral( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
