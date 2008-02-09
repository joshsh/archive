/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.UriMap;

import org.openrdf.model.vocabulary.OWL;

/**
 * RDF data and Java implementation of a library of primitive functions.
 */
public abstract class Library
{
	private static final RdfValue OWL_SAMEAS = new RdfValue( OWL.SAMEAS );

	public abstract void load( UriMap uf, ModelConnection mc )
		throws RippleException;

	protected PrimitiveStackRelation registerPrimitive( final Class c,
										final String name,
										final ModelConnection mc )
		throws RippleException
	{
		PrimitiveStackRelation prim;

		try
		{
			prim = (PrimitiveStackRelation) c.newInstance();
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
		final PrimitiveStackRelation primFinal = prim;

		Sink<RippleValue> aliasSink = new Sink<RippleValue>()
		{
			public void put( final RippleValue v )
				throws RippleException
			{
				bridge.add( v, primFinal, mc );
			}
		};

		// Add the primitive's stated URI to the map.
		bridge.add( prim, mc );

		// Add all stated aliases (but no aliases of aliases) to the map.
		mc.multiply( prim, OWL_SAMEAS, aliasSink, false );

		return prim;
	}
}

// kate: tab-width 4
