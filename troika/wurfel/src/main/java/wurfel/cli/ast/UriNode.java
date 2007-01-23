package wurfel.cli.ast;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.cli.Interpreter;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;

public class UriNode extends Ast
{
    private String value;

    public UriNode( final String value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp, EvaluationContext evalContext )
        throws WurfelException
    {
        return evalContext.createUri( value );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
