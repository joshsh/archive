/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.test.RippleTestCase;

public class RippleListTest extends RippleTestCase
{
	private class ListConcatenationTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			RippleValue
				a0 = new NumericLiteral( 42 ),
				a1 = new NumericLiteral( 137 ),
				a2 = new NumericLiteral( 23 ),
				b0 = new NumericLiteral( 216 );

			RippleList
				listA = new RippleList( a2 ).push( a1 ).push( a0 ),
				listB = new RippleList( b0 );

			assertEquals( listA.length(), 3 );
			assertEquals( listA.get( 0 ), a0 );
			assertEquals( listA.get( 1 ), a1 );
			assertEquals( listA.get( 2 ), a2 );
			assertEquals( listB.length(), 1 );
			assertEquals( listB.get( 0 ), b0 );
			assertEquals( RippleList.NIL.length(), 0 );

			RippleList lresult;

			lresult = RippleList.concat( listA, listB );
			assertEquals( lresult.length(), 4 );
			assertEquals( lresult.get( 0 ), a0 );
			assertEquals( lresult.get( 1 ), a1 );
			assertEquals( lresult.get( 2 ), a2 );
			assertEquals( lresult.get( 3 ), b0 );
			lresult = RippleList.concat( listB, listA );
			assertEquals( lresult.length(), 4 );
			assertEquals( lresult.get( 0 ), b0 );
			assertEquals( lresult.get( 1 ), a0 );
			assertEquals( lresult.get( 2 ), a1 );
			assertEquals( lresult.get( 3 ), a2 );

			lresult = RippleList.concat( listA, RippleList.NIL );
			assertEquals( lresult.length(), 3 );
			assertEquals( lresult.get( 0 ), a0 );
			assertEquals( lresult.get( 1 ), a1 );
			assertEquals( lresult.get( 2 ), a2 );

			lresult = RippleList.concat( RippleList.NIL, listB );
			assertEquals( lresult.length(), 1 );
			assertEquals( lresult.get( 0 ), b0 );

			lresult = RippleList.concat( RippleList.NIL, RippleList.NIL );
			assertEquals( lresult.length(), 0 );
			assertEquals( lresult, RippleList.NIL );
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new ListConcatenationTest() );
	}
}

// kate: tab-width 4
