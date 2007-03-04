package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;

public class BNodeNode extends Ast
{
    private String id;

    public BNodeNode( final String id )
    {
        this.id = id;
    }

    public String toString()
    {
        return "_:" + id;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        Value v = mc.createBNode( id );

        if ( null == v )
            throw new WurfelException( "blank node '" + toString() + "' does not exist" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
