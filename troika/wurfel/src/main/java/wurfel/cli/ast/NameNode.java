package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public class NameNode extends Ast
{
    private String localName;

    public NameNode( final String localName )
    {
        this.localName = localName;
    }

    public String toString()
    {
        return localName;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        Value v = itp.resolveUnqualifiedName( localName );

        if ( null == v )
            throw new WurfelException( "'" + localName + "' is undefined in this context" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
