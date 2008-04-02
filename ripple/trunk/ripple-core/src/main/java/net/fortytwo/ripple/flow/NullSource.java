/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Source;
import net.fortytwo.ripple.flow.Sink;

public class NullSource<T, E extends Exception> implements Source<T, E>
{
	public void writeTo( Sink<T, E> sink ) throws E
	{
	}
}
