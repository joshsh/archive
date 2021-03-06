package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public class QNameNode extends Ast
{
    private String nsPrefix, localName;

    public QNameNode( final String nsPrefix, final String localName )
    {
        this.nsPrefix = nsPrefix;
        this.localName = localName;
    }

    public String toString()
    {
        return ( ( null == nsPrefix ) ? "" : nsPrefix )
            + ":"
            + ( ( null == localName ) ? "" : localName );
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        Value v = itp.resolveQualifiedName( nsPrefix, localName );

        if ( null == v )
            throw new RippleException( "'" + toString() + "' is undefined in this context" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
