/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/model/StackRelation.java $
 * $Revision: 994 $
 * $Author: josh $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

public interface Mapping<D, R, E extends Exception>
{
	/**
	 * @return whether this function is referentially transparent w.r.t. all of its
	 * parameters.
	 */
	boolean isTransparent();

	void applyTo( D arg, Sink<R, E> sink ) throws E;
}
