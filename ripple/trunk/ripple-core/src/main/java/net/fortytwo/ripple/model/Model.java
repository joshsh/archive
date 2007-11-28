/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;

public interface Model
{
	ModelBridge getBridge();

	ModelConnection getConnection( String name ) throws RippleException;
	ModelConnection getConnection( String name, final LexiconUpdater updater ) throws RippleException;

	long countStatements() throws RippleException;
	
	Collection<ModelConnection> openConnections();
	void closeOpenConnections() throws RippleException;
	
	Dereferencer getDereferencer();
}
