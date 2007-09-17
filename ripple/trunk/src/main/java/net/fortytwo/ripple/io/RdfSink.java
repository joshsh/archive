/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public interface RdfSink
{
	void put( Statement st ) throws RippleException;
	void put( Namespace ns ) throws RippleException;
	void put( String comment ) throws RippleException;
}

// kate: tab-width 4
