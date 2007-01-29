package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import wurfel.model.EvaluationContext;

import org.openrdf.model.Value;

public class IdentifierNode extends Ast
{
    private String nsPrefix, localName;

    public IdentifierNode( final String localName )
    {
        nsPrefix = null;
        this.localName = localName;
    }

    public IdentifierNode( final String nsPrefix, final String localName )
    {
        this.nsPrefix = nsPrefix;
        this.localName = localName;
    }

    public Value evaluate( Interpreter itp, EvaluationContext evalContext )
        throws WurfelException
    {
        Value v = ( null == nsPrefix )
            ? itp.resolveUnqualifiedName( localName )
            : itp.resolveQualifiedName( nsPrefix, localName );

        if ( null == v )
            throw new WurfelException( "'" + ( ( null == nsPrefix ) ? "" : nsPrefix + "" ) + localName + "' is undefined in this context" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
