/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.test;

import junit.framework.TestCase;
import junit.framework.AssertionFailedError;

import org.openrdf.sail.Sail;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.util.RdfUtils;

public abstract class RippleTestCase extends TestCase
{
	protected static final boolean DEBUG = true;

	private int totalRunningTests;
	private Exception error = null;

	////////////////////////////////////////////////////////////////////////////

	protected abstract class TestRunnable implements Runnable
	{
		public abstract void test()
			throws Exception;

		public void run()
		{
			if ( DEBUG )
				System.out.println( this.getClass().getName() );

			try
			{
				test();
			}

			catch ( Exception t )
			{
				error = t;
			}

			finishTest();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void startTest()
	{
		totalRunningTests++;
	}

	private synchronized void finishTest()
	{
		totalRunningTests--;

		if ( 0 == totalRunningTests )
			notify();
	}

	protected void testSynchronous( TestRunnable r )
	{
		startTest();

		r.run();
	}

	protected void testAsynchronous( TestRunnable r )
	{
		startTest();

		Thread t = new Thread( r, "for RippleTestCase testAsynchronous" );
		t.start();
	}

	protected void start()
		throws RippleException
	{
		Ripple.initialize();

		totalRunningTests = 0;
	}

	protected synchronized void finish()
		throws Exception
	{
		try
		{
			while ( 0 < totalRunningTests )
			{
				wait();
				if ( null != error )
					break;
			}
		}

		catch ( InterruptedException e )
		{
			e.printStackTrace( System.err );
			fail( e.toString() );
		}

		if ( null != error )
			throw error;
	}

	public abstract void runTests()
		throws Exception;

	public void testAll()
		throws Exception
	{
		start();

		runTests();

		finish();
	}

	////////////////////////////////////////////////////////////////////////////

	private static Model model = null;

	protected static Model getTestModel()
		throws Exception
	{
		if ( null == model )
		{
			// Warning: we never call shutDown() on this repository.
			Sail sail = RdfUtils.createMemoryStoreSail();

			model = new Model( sail, "Unit Test Model" );
		}

		return model;
	}
}

// kate: tab-width 4
