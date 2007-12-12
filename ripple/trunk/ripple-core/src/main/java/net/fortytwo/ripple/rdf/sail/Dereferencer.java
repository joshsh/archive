/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.sail;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.rdf.RdfSink;

import org.openrdf.model.URI;

public interface Dereferencer
{
	void dereference( URI uri, RdfSink adderSink ) throws RippleException;

	void forget( RdfValue rv, ModelConnection mc )
		throws RippleException;

	void addSuccessMemo( String memo );
	void addFailureMemo( String memo );

	Collection<String> getSuccessMemos();
	Collection<String> getFailureMemos();
}

// kate: tab-width 4
