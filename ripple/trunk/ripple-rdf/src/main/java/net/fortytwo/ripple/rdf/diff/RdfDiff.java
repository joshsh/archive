/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfCollector;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfSource;

public class RdfDiff implements RdfDiffSink, RdfDiffSource
{
	private RdfCollector added, subtracted;

	public RdfDiff()
	{
		added = new RdfCollector();
		subtracted = new RdfCollector();
	}

	public RdfSink adderSink()
	{
		return added;
	}

	public RdfSink subtractorSink()
	{
		return subtracted;
	}

	public RdfSource adderSource()
	{
		return added;
	}

	public RdfSource subtractorSource()
	{
		return subtracted;
	}

	public void writeTo( final RdfDiffSink sink ) throws RippleException
	{
		added.writeTo( sink.adderSink() );
		subtracted.writeTo( sink.subtractorSink() );
	}
}

// kate: tab-width 4
