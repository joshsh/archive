/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;

import org.openrdf.model.vocabulary.OWL;

/**
 * RDF data and Java implementation of a library of primitive functions.
 */
public abstract class Extension
{
	private static final RdfValue OWL_SAMEAS = new RdfValue( OWL.SAMEAS );

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

		final ModelBridge bridge = mc.getModel().getBridge();
		final PrimitiveFunction primFinal = prim;

		Sink<RdfValue> aliasSink = new Sink<RdfValue>()
		{
			public void put( final RdfValue v )
				throws RippleException
			{
				bridge.add( v, primFinal );
			}
		};

		// Add the primitive's stated URI to the map.
		bridge.add( prim, mc );

		// Add all stated aliases (but no aliases of aliases) to the map.
		mc.multiply( prim.toRdf( mc ), OWL_SAMEAS, aliasSink );

		return prim;
	}
}

// kate: tab-width 4
