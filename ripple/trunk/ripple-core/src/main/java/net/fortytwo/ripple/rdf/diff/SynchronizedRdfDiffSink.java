/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SynchronizedRdfSink;

public class SynchronizedRdfDiffSink implements RdfDiffSink
{
	private RdfSink addSink, subSink;

	public SynchronizedRdfDiffSink( final RdfDiffSink sink, final Object synch )
	{
		addSink = new SynchronizedRdfSink( sink.adderSink(), synch );
		subSink = new SynchronizedRdfSink( sink.subtractorSink(), synch );
	}

	public RdfSink adderSink()
	{
		return addSink;
	}

	public RdfSink subtractorSink()
	{
		return subSink;
	}
}

// kate: tab-width 4
