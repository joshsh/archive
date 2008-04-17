/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RDFCollector;
import net.fortytwo.ripple.rdf.RDFSink;
import net.fortytwo.ripple.rdf.RDFSource;

public class RdfDiff implements RdfDiffSink, RdfDiffSource
{
	private RDFCollector added, subtracted;

	public RdfDiff()
	{
		added = new RDFCollector();
		subtracted = new RDFCollector();
	}

	public RDFSink adderSink()
	{
		return added;
	}

	public RDFSink subtractorSink()
	{
		return subtracted;
	}

	public RDFSource adderSource()
	{
		return added;
	}

	public RDFSource subtractorSource()
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
