package net.fortytwo.ripple.cli.ast;

import java.util.Iterator;

import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class TypedLiteralAst implements Ast
{
	private String value;
	private Ast type;

	public TypedLiteralAst( final String value, Ast type )
	{
		this.value = value;
		this.type = type;
	}

	public void evaluate( Sink<RippleValue> sink,
						Interpreter itp,
						ModelConnection mc )
		throws RippleException
	{
		ContainerSink values = new ContainerSink();
		type.evaluate( values, itp, mc );
		for ( Iterator<RippleValue> iter = values.iterator(); iter.hasNext(); )
		{
			RippleValue typeValue = iter.next();

			if ( null == typeValue )
				throw new RippleException( "badly typed literal" );

			Value v = typeValue.toRdf( mc ).getRdfValue();

			if ( !( v instanceof URI ) )
				throw new RippleException( "literal type is not a URI" );
			else
				sink.put( mc.createValue( value, (URI) v ) );
		}
	}

	public String toString()
	{
		return "\"" + value + "\"^^" + type;
	}
}

// kate: tab-width 4
