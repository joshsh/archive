package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public class IdentifierNode extends Ast
{
    private String value;

    public IdentifierNode( final String value )
    {
        this.value = value;
    }

    public Value evaluate( Interpreter itp )
        throws WurfelException
    {
        Value v = itp.resolveIdentifier( value );

        if ( null == v )
            throw new WurfelException( "'" + value + "' is undefined in this context" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
