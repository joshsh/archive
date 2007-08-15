/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.test.RippleTestCase;

public class ModelTest extends RippleTestCase
{
	private class ModelConstructorTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new ModelConstructorTest() );
	}
}

// kate: tab-width 4
