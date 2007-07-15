package net.fortytwo.ripple.extensions.etc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.Literal;
import org.openrdf.model.URI;

public class Type extends PrimitiveFunction
{
	public Type()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
								Sink<RippleList> sink,
								ModelConnection mc )
		throws RippleException
	{
		Value v;

		v = stack.getFirst().toRdf( mc ).getRdfValue();
		stack = stack.getRest();

		if ( v instanceof Literal )
		{
			URI type = ( (Literal) v ).getDatatype();

			if ( null != type )
				sink.put( new RippleList( new RdfValue( type ), stack ) );
		}
	}
}

// kate: tab-width 4
