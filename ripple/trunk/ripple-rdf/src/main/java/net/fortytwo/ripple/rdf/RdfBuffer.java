/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/src/main/java/net/fortytwo/ripple/rdf/diff/RdfDiffBuffer.java $
 * $Revision: 906 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.RippleException;

public class RdfBuffer extends RdfCollector
{
	private RdfSink sink;

	public RdfBuffer( final RdfSink sink )
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
