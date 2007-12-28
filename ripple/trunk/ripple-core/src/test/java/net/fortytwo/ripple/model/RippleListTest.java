/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.InputStream;
import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfImporter;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.rio.RDFFormat;

public class RippleListTest extends RippleTestCase
{
	private class FromRdfTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for FromRdfTest" );

			InputStream is = RippleListTest.class.getResourceAsStream( "listTest.ttl" );
			RdfImporter importer = new RdfImporter( mc );
			SesameInputAdapter sc = new SesameInputAdapter( importer );
			RdfUtils.read( is, sc, "", RDFFormat.TURTLE );
			mc.commit();
			is.close();

			RdfValue head;
			Collector<RippleList> created = new Collector<RippleList>();
			final Collector<RippleList> allowed = new Collector<RippleList>();
			
			Sink<RippleList> verifySink = new Sink<RippleList>()
			{
				public void put( final RippleList list ) throws RippleException
				{
					boolean found = false;
					
					for ( Iterator<RippleList> iter = allowed.iterator(); iter.hasNext(); )
					{
						if ( 0 == iter.next().compareTo( list ) )
						{
							found = true;
							break;
						}
					}
					
					assertTrue( found );
				}
			};
			
			RippleValue l1 = mc.value( "1" );
			RippleValue l2 = mc.value( "2" );
			RippleValue l1a = mc.value( "1a" );
			RippleValue l1b = mc.value( "1b" );		
			RippleValue l2a = mc.value( "2a" );
			RippleValue l2b = mc.value( "2b" );

			head = new RdfValue( mc.createUri( "urn:test.RippleListTest.FromRdfTest#simpleList" ) );
			created.clear();
			mc.getLists( head, created );
			assertEquals( 1, created.size() );
			allowed.clear();
			allowed.put( mc.list( l2 ).push( l1 ) );
			created.writeTo( verifySink );
			
			head = new RdfValue( mc.createUri( "urn:test.RippleListTest.FromRdfTest#firstBranchingList" ) );
			created.clear();
			mc.getLists( head, created );
			assertEquals( 2, created.size() );
			allowed.clear();
			allowed.put( mc.list( l2 ).push( l1a ) );
			allowed.put( mc.list( l2 ).push( l1b ) );
			created.writeTo( verifySink );

			head = new RdfValue( mc.createUri( "urn:test.RippleListTest.FromRdfTest#restBranchingList" ) );
			created.clear();
			mc.getLists( head, created );
			assertEquals( 2, created.size() );
			allowed.clear();
			allowed.put( mc.list( l2a ).push( l1 ) );
			allowed.put( mc.list( l2b ).push( l1 ) );
			created.writeTo( verifySink );
			
			head = new RdfValue( mc.createUri( "urn:test.RippleListTest.FromRdfTest#firstAndRestBranchingList" ) );
			created.clear();
			mc.getLists( head, created );
			assertEquals( 4, created.size() );
			allowed.clear();
			allowed.put( mc.list( l2a ).push( l1a ) );
			allowed.put( mc.list( l2a ).push( l1b ) );
			allowed.put( mc.list( l2b ).push( l1a ) );
			allowed.put( mc.list( l2b ).push( l1b ) );
			created.writeTo( verifySink );
			
			// Note: the circular list is not tested.
			
			mc.close();
		}
	}
	
	private class ListConcatenationTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for ListConcatenationTest" );
			RippleValue
				a0 = mc.value( 42 ),
				a1 = mc.value( 137 ),
				a2 = mc.value( 23 ),
				b0 = mc.value( 216 );
			mc.close();

			RippleList
				listA = mc.list( a2 ).push( a1 ).push( a0 ),
				listB = mc.list( b0 );

			assertEquals( listA.length(), 3 );
			assertEquals( listA.get( 0 ), a0 );
			assertEquals( listA.get( 1 ), a1 );
			assertEquals( listA.get( 2 ), a2 );
			assertEquals( listB.length(), 1 );
			assertEquals( listB.get( 0 ), b0 );
			assertEquals( RippleList.NIL.length(), 0 );

			RippleList lresult;

			lresult = mc.concat( listA, listB );
			assertEquals( lresult.length(), 4 );
			assertEquals( lresult.get( 0 ), a0 );
			assertEquals( lresult.get( 1 ), a1 );
			assertEquals( lresult.get( 2 ), a2 );
			assertEquals( lresult.get( 3 ), b0 );
			lresult = mc.concat( listB, listA );
			assertEquals( lresult.length(), 4 );
			assertEquals( lresult.get( 0 ), b0 );
			assertEquals( lresult.get( 1 ), a0 );
			assertEquals( lresult.get( 2 ), a1 );
			assertEquals( lresult.get( 3 ), a2 );

			lresult = mc.concat( listA, RippleList.NIL );
			assertEquals( lresult.length(), 3 );
			assertEquals( lresult.get( 0 ), a0 );
			assertEquals( lresult.get( 1 ), a1 );
			assertEquals( lresult.get( 2 ), a2 );

			lresult = mc.concat( RippleList.NIL, listB );
			assertEquals( lresult.length(), 1 );
			assertEquals( lresult.get( 0 ), b0 );

			lresult = mc.concat( RippleList.NIL, RippleList.NIL );
			assertEquals( lresult.length(), 0 );
			assertEquals( lresult, RippleList.NIL );
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new ListConcatenationTest() );
		testAsynchronous( new FromRdfTest() );
	}
}

// kate: tab-width 4
