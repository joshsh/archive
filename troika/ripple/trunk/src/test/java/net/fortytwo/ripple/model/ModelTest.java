package net.fortytwo.ripple.model;

import net.fortytwo.ripple.model.Model;
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
		throws Throwable
	{
		testAsynchronous( new ModelConstructorTest() );
	}
}

// kate: tab-width 4
