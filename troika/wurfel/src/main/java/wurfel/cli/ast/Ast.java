package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;

public abstract class Ast
{
    public abstract Value evaluate( Interpreter itp, EvaluationContext evalContext )
        throws WurfelException;

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
