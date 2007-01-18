package wurfel.cli.ast;

import wurfel.Context;
import wurfel.cli.Interpreter;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public class DoubleNode extends Ast
{
    private double value;

    public DoubleNode( final double value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp )
        throws WurfelException
    {
        Context context = itp.getContext();

        return context.createLiteral( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
