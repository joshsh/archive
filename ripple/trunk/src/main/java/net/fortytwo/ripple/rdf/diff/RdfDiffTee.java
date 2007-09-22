package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;

import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfTee;

public class RdfDiffTee implements RdfDiffSink
{
	private RdfTee adderTee, subtractorTee;

	public RdfDiffTee( final RdfDiffSink sinkA, final RdfDiffSink sinkB )
	{
		adderTee = new RdfTee( sinkA.adderSink(), sinkB.adderSink() );
		subtractorTee = new RdfTee( sinkA.subtractorSink(), sinkB.subtractorSink() );
	}

	public RdfSink adderSink()
	{
		return adderTee;
	}

	public RdfSink subtractorSink()
	{
		return subtractorTee;
	}
}

// kate: tab-width 4
