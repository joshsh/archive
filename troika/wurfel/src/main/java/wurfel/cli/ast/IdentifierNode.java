package wurfel.cli.ast;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

public class IdentifierNode extends Ast
{
    private String value;

    public IdentifierNode( final String value )
    {
        this.value = value;
    }

    public Value evaluate( Context context )
        throws WurfelException
    {
        Value v = context.resolveIdentifier( value );

        if ( null == v )
            throw new WurfelException( "'" + value + "' is undefined in this context" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
