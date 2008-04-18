/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.tools;

public class XPComTest
{
	private int count;

	public static String getGreetings()
	{
		return "Hello World!";
	}

	public XPComTest( final int start )
	{
		count = start;
	}

	public void count()
	{
		count++;
	}

	public int getCount()
	{
		return count;
	}
}

// kate: tab-width 4
