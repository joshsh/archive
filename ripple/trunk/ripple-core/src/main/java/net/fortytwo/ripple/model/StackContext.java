/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Feb 7, 2008
 * Time: 6:17:24 PM
 */
public class StackContext implements Cloneable
{
	protected RippleList stack;
	protected ModelConnection modelConnection;

	public StackContext( final RippleList stack, final ModelConnection mc )
	{
		this.stack = stack;
		this.modelConnection = mc;
	}

	public RippleList getStack()
	{
		return this.stack;
	}
	
	public ModelConnection getModelConnection()
	{
		return this.modelConnection;
	}

	public StackContext with( final RippleList s ) {
		try
		{
			StackContext clone = (StackContext) this.clone();
			clone.stack = s;
			return clone;
		}

		// This shouldn't happen.
		catch ( CloneNotSupportedException e )
		{
			new RippleException( e ).logError();
			return this;
		}
	}
}
