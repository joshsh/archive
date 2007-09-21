package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

public interface RdfDiffSource
{
	RdfSource adderSource();
	RdfSource subtractorSource();
	void writeTo( RdfDiffSink sink ) throws RippleException;
}

// kate: tab-width 4
