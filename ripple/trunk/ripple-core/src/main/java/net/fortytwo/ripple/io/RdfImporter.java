/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;

public class RdfImporter implements RdfSink
{
	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> cmtSink;

	public RdfImporter( final ModelConnection mc,
						final Resource... contexts )
	{
		final boolean override = Ripple.preferNewestNamespaceDefinitions();

		stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				mc.add( st, contexts );
			}
		};

		nsSink = new Sink<Namespace>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				mc.setNamespace( ns.getPrefix(), ns.getName(), override );
			}
		};

		cmtSink = new Sink<String>()
		{
			public void put( final String comment ) throws RippleException
			{
			}
		};
	}

	public Sink<Statement> statementSink()
	{
		return stSink;
	}

	public Sink<Namespace> namespaceSink()
	{
		return nsSink;
	}

	public Sink<String> commentSink()
	{
		return cmtSink;
	}
}

// kate: tab-width 4
