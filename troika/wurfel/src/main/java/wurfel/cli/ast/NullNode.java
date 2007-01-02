package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

/**
 *  Warning: use this class with caution.
 */
public class NullNode extends Ast
{
    public NullNode()
    { }

    public Value evaluate( Context context )
        throws WurfelException
    {
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
