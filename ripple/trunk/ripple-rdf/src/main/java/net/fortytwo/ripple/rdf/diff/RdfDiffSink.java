/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-utils/src/main/java/net/fortytwo/ripple/rdf/diff/RdfDiffSink.java $
 * $Revision: 1042 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import net.fortytwo.ripple.rdf.RdfSink;

public interface RdfDiffSink
{
	RdfSink adderSink();
	RdfSink subtractorSink();
}

// kate: tab-width 4
