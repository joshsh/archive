package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public abstract class Ast
{
    public abstract Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException;

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
