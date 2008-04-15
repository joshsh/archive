/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/diff/RdfDiffBuffer.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;

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
