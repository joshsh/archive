/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.model.Resource;
import org.openrdf.model.ValueFactory;

import net.fortytwo.ripple.util.Sink;

public class SingleContextPipe implements RdfSink
{
	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> cmtSink;

	public SingleContextPipe( final RdfSink sink,
							final Resource context,
							final ValueFactory valueFactory )
	{
		final Sink<Statement> otherStSink = sink.statementSink();

		stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				Statement newSt;
				
				try
				{
					newSt = valueFactory.createStatement(
							st.getSubject(), st.getPredicate(), st.getObject(),	context );
				}
				
				catch ( Throwable t )
				{
					throw new RippleException( t );
				}
				
				otherStSink.put( newSt );
			}
		};

		nsSink = sink.namespaceSink();
		cmtSink = sink.commentSink();
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
