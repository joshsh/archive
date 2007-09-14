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
	public abstract void load( UrlFactory uf, ModelConnection mc )
		throws RippleException;

	static boolean initialized = false;
	static RdfValue owlSameAs;

	protected PrimitiveFunction registerPrimitive( final Class c,
										final String name,
										final ModelConnection mc )
		throws RippleException
	{
		PrimitiveFunction prim;

		if ( !initialized )
		{
			owlSameAs = new RdfValue( OWL.SAMEAS );
			initialized = true;
		}

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
		mc.multiply( prim.toRdf( mc ), owlSameAs, aliasSink );

		return prim;
	}
}

// kate: tab-width 4
