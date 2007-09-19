/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

public abstract class RdfSink
{
	public abstract void put( Statement st ) throws RippleException;
	public abstract void put( Namespace ns ) throws RippleException;
	public abstract void put( String comment ) throws RippleException;

	public void putStatements( final Source<Statement> source ) throws RippleException
	{
		final RdfSink rdfSink = this;

		Sink<Statement> sink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				rdfSink.put( st );
			}
		};

		source.writeTo( sink );
	}

	public void putNamespaces( final Source<Namespace> source ) throws RippleException
	{
		final RdfSink rdfSink = this;

		Sink<Namespace> sink = new Sink<Namespace>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				rdfSink.put( ns );
			}
		};

		source.writeTo( sink );
	}

	public void putComments( final Source<String> source ) throws RippleException
	{
		final RdfSink rdfSink = this;

		Sink<String> sink = new Sink<String>()
		{
			public void put( final String comment ) throws RippleException
			{
				rdfSink.put( comment );
			}
		};

		source.writeTo( sink );
	}
}

// kate: tab-width 4
