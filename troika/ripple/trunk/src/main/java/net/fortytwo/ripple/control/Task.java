/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.control;

import net.fortytwo.ripple.RippleException;

public interface Task
{
	public void execute() throws RippleException;

//	public void stop();
}

// kate: tab-width 4
