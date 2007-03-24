package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;

public class BNodeNode implements Ast
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

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        RippleValue v = new RdfValue( mc.createBNode( id ) );

        if ( null == v )
            throw new RippleException( "blank node '" + this + "' does not exist" );
        else
            return v;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
