package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SynchronizedRdfSink;

public class SynchronizedRdfDiffSink implements RdfDiffSink
{
	private RdfSink addSink, subSink;

	public SynchronizedRdfDiffSink( final RdfDiffSink sink )
	{
		addSink = new SynchronizedRdfSink( sink.adderSink() );
		subSink = new SynchronizedRdfSink( sink.subtractorSink() );
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
