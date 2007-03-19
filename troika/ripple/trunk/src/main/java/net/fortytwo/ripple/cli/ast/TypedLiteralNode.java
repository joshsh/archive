package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

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
	Value v = typeValue.toRdf( mc ).getRdfValue();
	if ( !( v instanceof URI ) )
		throw new RippleException( "literal type is not a URI" );
        return mc.createValue( value, (URI) v );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
