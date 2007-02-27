package wurfel.cli.ast;

import wurfel.cli.Interpreter;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.ModelConnection;

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

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws WurfelException
    {
        Value typeValue = type.evaluate( itp, mc );
        if ( null == typeValue || !( typeValue instanceof URI ) )
            throw new WurfelException( "badly typed literal" );

        return mc.createLiteral( value, (URI) typeValue );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
