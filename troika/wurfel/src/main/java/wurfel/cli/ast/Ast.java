package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public abstract class Ast
{
    public abstract Value evaluate( Context context )
        throws WurfelException;

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
