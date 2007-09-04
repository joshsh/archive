/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.net.URL;

import net.fortytwo.ripple.io.RdfCollector;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.RdfUtils;

import org.openrdf.rio.RDFFormat;

public class RdfFormatsTest extends RippleTestCase
{
	RdfCollector collector;
	SesameAdapter sesameAdapter;

	private class ParseN3Test extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.n3" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.N3 );
			assertEquals( collector.countStatements(), 4 );
			assertEquals( collector.countNamespaces(), 2 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	private class ParseNtriplesTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.nt" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.NTRIPLES );
			assertEquals( collector.countStatements(), 3 );
			assertEquals( collector.countNamespaces(), 0 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	private class ParseRdfXmlTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.rdf" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.RDFXML );
			assertEquals( collector.countStatements(), 4 );
			assertEquals( collector.countNamespaces(), 3 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	private class ParseTrigTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.trig" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.TRIG );
			assertEquals( collector.countStatements(), 6 );
			assertEquals( collector.countNamespaces(), 2 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	private class ParseTrixTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.trix" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.TRIX );
			assertEquals( collector.countStatements(), 3 );
			assertEquals( collector.countNamespaces(), 0 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	private class ParseTurtleTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			collector.clear();

			URL url = RdfFormatsTest.class.getResource( "rdfformatstest.ttl" );
			RDFFormat format = RdfUtils.read( url, sesameAdapter, url.toString() );

			assertEquals( format, RDFFormat.TURTLE );
			assertEquals( collector.countStatements(), 3 );
			assertEquals( collector.countNamespaces(), 3 );
//			assertEquals( collector.countComments(), 0 );
		}
	}

	public void runTests()
		throws Exception
	{
		collector = new RdfCollector();
		sesameAdapter = new SesameAdapter( collector );

		testSynchronous( new ParseN3Test() );
		testSynchronous( new ParseNtriplesTest() );
		testSynchronous( new ParseRdfXmlTest() );
// FIXME
//		testSynchronous( new ParseTrigTest() );
		testSynchronous( new ParseTrixTest() );
		testSynchronous( new ParseTurtleTest() );
	}
}

// kate: tab-width 4
