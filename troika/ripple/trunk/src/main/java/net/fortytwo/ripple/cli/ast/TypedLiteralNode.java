package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

public class TypedLiteralNode extends Ast
{
    private String value;
    private Ast type;

    public TypedLiteralNode( final String value, Ast type )
    {
        this.value = value;
        this.type = type;
    }

    public RippleValue evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        RippleValue typeValue = type.evaluate( itp, mc );
        if ( null == typeValue )
            throw new RippleException( "badly typed literal" );

        return mc.createLiteral( value, typeValue );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
