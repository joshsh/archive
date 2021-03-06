package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.ModelConnection;

import org.openrdf.model.Value;

public class StringNode extends Ast
{
    private String value, language;

    public StringNode( final String value )
    {
        this.value = value;
        this.language = null;
    }

    public StringNode( final String value, final String language )
    {
        this.value = value;
        this.language = language;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        return ( null == language )
            ? mc.createLiteral( value )
            : mc.createLiteral( value, language );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
