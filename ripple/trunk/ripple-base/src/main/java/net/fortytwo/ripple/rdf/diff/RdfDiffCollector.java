/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfCollector;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfSource;

public class RdfDiffCollector implements RdfDiffSource, RdfDiffSink
{
	private RdfCollector adderCollector, subtractorCollector;

	public RdfDiffCollector()
	{
		adderCollector = new RdfCollector();
		subtractorCollector = new RdfCollector();
	}

	public RdfSink adderSink()
	{
		return adderCollector;
	}

	public RdfSink subtractorSink()
	{
		return subtractorCollector;
	}

	public RdfSource adderSource()
	{
		return adderCollector;
	}

	public RdfSource subtractorSource()
	{
		return subtractorCollector;
	}

	public void writeTo( final RdfDiffSink sink ) throws RippleException
	{
		adderCollector.writeTo( sink.adderSink() );
		subtractorCollector.writeTo( sink.subtractorSink() );
	}

	public void clear()
	{
		adderCollector.clear();
		subtractorCollector.clear();
	}
}

// kate: tab-width 4
