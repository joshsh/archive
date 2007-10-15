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

public class RdfDiffBuffer extends RdfDiffCollector
{
	private RdfDiffSink sink;

	public RdfDiffBuffer( final RdfDiffSink sink )
	{
		super();
		this.sink = sink;
	}

	public void flush() throws RippleException
	{
		writeTo( sink );
		clear();
	}
}

// kate: tab-width 4
