package net.fortytwo.ripple.extensions.newstuff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Scrap extends PrimitiveFunction
{
	public Scrap( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public void applyTo( RippleStack stack,
						Sink<RippleStack> sink,
						ModelConnection mc )
		throws RippleException
	{
		// Do nothing.
	}
}

// kate: tab-width 4
