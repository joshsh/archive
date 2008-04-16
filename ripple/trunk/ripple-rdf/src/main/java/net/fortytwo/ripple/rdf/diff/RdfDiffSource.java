/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/diff/RdfDiffSource.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.RippleException;

import net.fortytwo.ripple.rdf.RdfSource;

public interface RdfDiffSource
{
	RdfSource adderSource();
	RdfSource subtractorSource();
	void writeTo( RdfDiffSink sink ) throws RippleException;
}

// kate: tab-width 4
