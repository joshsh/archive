/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Statement;

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
