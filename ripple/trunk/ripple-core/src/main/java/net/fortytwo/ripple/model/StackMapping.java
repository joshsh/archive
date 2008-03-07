/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

public interface StackMapping extends Mapping<StackContext, StackContext>
{
	/**
	*  The fixed number of arguments which this function consumes before
	*  yielding a result.
	*/
	int arity();
}
