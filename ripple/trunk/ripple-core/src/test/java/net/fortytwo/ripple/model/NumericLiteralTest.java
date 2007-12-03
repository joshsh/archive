/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.model.impl.sesame.NumericLiteralImpl;
import net.fortytwo.ripple.test.RippleTestCase;

public class NumericLiteralTest extends RippleTestCase
{
	private class TypeTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			NumericLiteral
				intLit = new NumericLiteralImpl( 5 ),
				doubleLit = new NumericLiteralImpl( 3.1415926 );

			assertEquals( intLit.getType(), NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals( doubleLit.getType(), NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				intLit.abs().getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				doubleLit.abs().getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			/*
			assertEquals(
				NumericLiteral.neg( intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.neg( doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.add( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.add( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.add( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.add( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.sub( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.sub( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.sub( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.sub( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.mul( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.mul( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.mul( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.mul( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.div( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.div( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.div( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.div( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.mod( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.mod( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.mod( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.mod( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );

			assertEquals(
				NumericLiteral.pow( intLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.INTEGER );
			assertEquals(
				NumericLiteral.pow( intLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.pow( doubleLit, intLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE );
			assertEquals(
				NumericLiteral.pow( doubleLit, doubleLit ).getType(),
				NumericLiteral.NumericLiteralType.DOUBLE ); */
		}
	}
	
	private class EqualityTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			// int == int
			assertEquals( new NumericLiteralImpl( 1 ), new NumericLiteralImpl( 1 ) );
			assertEquals( new NumericLiteralImpl( -1 ), new NumericLiteralImpl( -1 ) );
			assertEquals( new NumericLiteralImpl( 0 ), new NumericLiteralImpl( 0 ) );
			
			// long == long
			assertEquals( new NumericLiteralImpl( 1l ), new NumericLiteralImpl( 1l ) );
			assertEquals( new NumericLiteralImpl( -1l ), new NumericLiteralImpl( -1l ) );
			assertEquals( new NumericLiteralImpl( 0l ), new NumericLiteralImpl( 0l ) );
			
			// double == double
			assertEquals( new NumericLiteralImpl( 1.0 ), new NumericLiteralImpl( 1.0 ) );
			assertEquals( new NumericLiteralImpl( -1.0 ), new NumericLiteralImpl( -1.0 ) );
			assertEquals( new NumericLiteralImpl( 0.0 ), new NumericLiteralImpl( 0.0 ) );
			
			// mixed comparisons
			assertEquals( new NumericLiteralImpl( 1 ), new NumericLiteralImpl( 1l ) );
			assertEquals( new NumericLiteralImpl( 1 ), new NumericLiteralImpl( 1.0 ) );
			assertEquals( new NumericLiteralImpl( 1l ), new NumericLiteralImpl( 1 ) );
			assertEquals( new NumericLiteralImpl( 1l ), new NumericLiteralImpl( 1.0 ) );
			assertEquals( new NumericLiteralImpl( 1.0 ), new NumericLiteralImpl( 1 ) );
			assertEquals( new NumericLiteralImpl( 1.0 ), new NumericLiteralImpl( 1l ) );			
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new TypeTest() );
		testAsynchronous( new EqualityTest() );
	}
}

// kate: tab-width 4
