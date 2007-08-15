/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public interface RdfSink
{
	public void put( Statement st ) throws RippleException;
	public void put( Namespace ns ) throws RippleException;
	public void put( String comment ) throws RippleException;
}

// kate: tab-width 4
