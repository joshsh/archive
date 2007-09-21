package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

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
