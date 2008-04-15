/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/test/java/net/fortytwo/ripple/test/RippleTestCase.java $
 * $Revision: 1027 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.test;

import junit.framework.TestCase;

import org.openrdf.sail.Sail;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Collector;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.impl.sesame.SesameModel;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.linkeddata.sail.LinkedDataSail;
import net.fortytwo.ripple.util.UriMap;

import java.util.Iterator;
import java.util.Arrays;

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

	private static Sail sail = null;
	private static Model model = null;
	private static UriMap uriMap = null;
	
	protected static Sail getTestSail() throws Exception
	{
		if ( null == sail )
		{
			// Warning: we never call shutDown() on this Sail.
			sail = RdfUtils.createMemoryStoreSail();
		}
		
		return sail;
	}
	
	protected static Model getTestModel()
		throws Exception
	{
		if ( null == model )
		{
			uriMap = new UriMap();
			LinkedDataSail ldSail = new LinkedDataSail( getTestSail(), uriMap );
			ldSail.initialize();
			model = new SesameModel( ldSail, uriMap );
		}

		return model;
	}
	
	protected static UriMap getTestUrlFactory() throws Exception
	{
		getTestModel();
		return uriMap;
	}

	public static RippleList createStack( final ModelConnection mc, final RippleValue... values )
	{
		if ( 0 == values.length )
		{
			return RippleList.NIL;
		}

		RippleList l = mc.list( values[0] );
		for ( int i = 1; i < values.length; i++ )
		{
			l = l.push( values[i] );
		}

		return l;
	}

	public static RippleList createQueue( final ModelConnection mc, final RippleValue... values )
	{
		return mc.invert( createStack( mc, values ) );
	}

	public static void assertCollectorsEqual( Collector<RippleList, RippleException> expected, Collector<RippleList, RippleException> actual ) throws Exception
	{
//System.out.println("expected: " + expected + ", actual = " + actual);
        int size = expected.size();
		assertEquals( size, actual.size() );
		if ( 0 == size )
		{
			return;
		}

		RippleList[] expArray = new RippleList[size];
		RippleList[] actArray = new RippleList[size];
		Iterator<RippleList> expIter = expected.iterator();
		Iterator<RippleList> actIter = actual.iterator();
		for ( int i = 0; i < size; i++ )
		{
			expArray[i] = expIter.next();
			actArray[i] = actIter.next();
		}

		Arrays.sort( expArray );
		Arrays.sort( actArray );
		for ( int i = 0; i < size; i++ )
		{
//System.out.println("expected: " + expArray[i] + ", actual = " + actArray[i]);
			assertEquals( expArray[i], actArray[i] );
		}
	}
}

// kate: tab-width 4
