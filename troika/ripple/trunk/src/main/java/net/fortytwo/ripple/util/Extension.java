package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;

public abstract class Extension
{
	public abstract void load( UrlFactory uf, ModelConnection mc )
		throws RippleException;

	protected PrimitiveFunction registerPrimitive( final Class c,
										final String name,
										final ModelConnection mc )
		throws RippleException
	{
		PrimitiveFunction prim;

		try
		{
			prim = (PrimitiveFunction) c.newInstance();
			prim.setRdfEquivalent( new RdfValue( mc.createUri( name ) ), mc );
		}

		catch ( InstantiationException e )
		{
			throw new RippleException( e );
		}

		catch ( IllegalAccessException e )
		{
			throw new RippleException( e );
		}

		mc.getModel().getBridge().add( prim, mc );

		return prim;
	}
}

// kate: tab-width 4
