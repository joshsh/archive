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
