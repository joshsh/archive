package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;

/**
 *  Warning: use this class with caution.
 */
public class NullNode extends Ast
{
    public NullNode()
    { }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
