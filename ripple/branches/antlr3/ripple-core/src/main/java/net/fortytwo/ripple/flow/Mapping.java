/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/model/StackRelation.java $
 * $Revision: 994 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;

// kate: tab-width 4

public interface Mapping<D, R, E extends Exception>
{
	/**
	 * @return whether this function is referentially transparent w.r.t. all of its
	 * parameters.
	 */
	boolean isTransparent();

	void applyTo( D arg, Sink<R, E> sink ) throws E;
}