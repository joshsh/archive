/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.StringTokenizer;

import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.FileUtils;

public class ModelTest extends RippleTestCase
{
	private class ModelConstructorTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			getTestModel();
		}
	}
	
	private class NumericLiteralTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
			ModelConnection mc = model.getConnection( "for NumericLiteralTest" );
			NumericLiteral l;
			
			// Create an integer literal.
			l = mc.value( 42 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.INTEGER, l.getType() );
			assertEquals( 42, l.intValue() );
			l = mc.value( 0 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.INTEGER, l.getType() );
			assertEquals( 0, l.intValue() );
			l = mc.value( -42 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.INTEGER, l.getType() );
			assertEquals( -42, l.intValue() );

			// Create a long literal.
			l = mc.value( 42l );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.LONG, l.getType() );
			assertEquals( 42l, l.longValue() );
			l = mc.value( 0l );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.LONG, l.getType() );
			assertEquals( 0l, l.longValue() );
			l = mc.value( -42l );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.LONG, l.getType() );
			assertEquals( -42l, l.longValue() );
			
			// Create a double literal
			l = mc.value( 42.0 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.DOUBLE, l.getType() );
			assertEquals( 42.0, l.doubleValue() );
			l = mc.value( 0.0 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.DOUBLE, l.getType() );
			assertEquals( 0.0, l.doubleValue() );
			l = mc.value( -42.0 );
			assertTrue( l instanceof NumericLiteral );
			assertEquals( NumericLiteral.NumericLiteralType.DOUBLE, l.getType() );
			assertEquals( -42.0, l.doubleValue() );
			
			InputStream is = ModelTest.class.getResourceAsStream( "numericLiteralTest.txt" );
			Iterator<String> lines = FileUtils.getLines( is ).iterator();
			is.close();
			Map<String, Integer> argsForFunc = new HashMap<String, Integer>();
			argsForFunc.put( "abs", new Integer( 1 ) );
			argsForFunc.put( "neg", new Integer( 1 ) );
			argsForFunc.put( "add", new Integer( 2 ) );
			argsForFunc.put( "sub", new Integer( 2 ) );
			argsForFunc.put( "mul", new Integer( 2 ) );
			argsForFunc.put( "div", new Integer( 2 ) );
			argsForFunc.put( "mod", new Integer( 2 ) );
			argsForFunc.put( "pow", new Integer( 2 ) );
			
			// Verify individual operator test cases.
			while ( lines.hasNext() )
			{
				StringTokenizer tokenizer = new StringTokenizer(
						lines.next(), " \t" );
				String func = tokenizer.nextToken();
				String signature = func + "(";
				int argv = argsForFunc.get( func );
				NumericLiteral[] args = new NumericLiteral[argv];
				for ( int i = 0; i < argv; i++)
				{
					String s = tokenizer.nextToken();
					if ( i > 0 )
					{
						signature += ", ";
					}
					signature += s;
					args[i] = createNumericLiteral( s );
				}
				signature += ")";
				
				// Skip the '=' token
				tokenizer.nextToken();
				
				NumericLiteral correctResult = createNumericLiteral( tokenizer.nextToken() );
				NumericLiteral actualResult = null;

				Throwable thrown = null;
				
				try
				{
					if ( func.equals( "abs" ) )
					{
						actualResult = NumericLiteral.abs( args[0] );
					}

					else if ( func.equals( "neg" ) )
					{
						actualResult = NumericLiteral.neg( args[0] );
					}

					else if ( func.equals( "add" ) )
					{
						actualResult = NumericLiteral.add( args[0], args[1] );
					}

					else if ( func.equals( "sub" ) )
					{
						actualResult = NumericLiteral.sub( args[0], args[1] );
					}

					else if ( func.equals( "mul" ) )
					{
						actualResult = NumericLiteral.mul( args[0], args[1] );
					}

					else if ( func.equals( "div" ) )
					{
						actualResult = NumericLiteral.div( args[0], args[1] );
					}

					else if ( func.equals( "mod" ) )
					{
						actualResult = NumericLiteral.mod( args[0], args[1] );
					}

					else if ( func.equals( "pow" ) )
					{
						actualResult = NumericLiteral.pow( args[0], args[1] );
					}

					else
					{
						throw new Exception( "bad function: " + func );
					}
				}
				
				catch ( Throwable t )
				{
					thrown = t;
				}
				
				if ( null == thrown )
				{
					assertTrue( "for case " + signature, null != correctResult );
					
					switch ( correctResult.getType() )
					{
					case INTEGER:
						assertEquals( "for case " + signature, NumericLiteral.NumericLiteralType.INTEGER, actualResult.getType() );
						assertEquals( "for case " + signature, correctResult.intValue(), actualResult.intValue() );
						break;
					case LONG:
						assertEquals( "for case " + signature, NumericLiteral.NumericLiteralType.LONG, actualResult.getType() );
						assertEquals( "for case " + signature, correctResult.longValue(), actualResult.longValue() );
						break;
					case DOUBLE:
						assertEquals( "for case " + signature, NumericLiteral.NumericLiteralType.DOUBLE, actualResult.getType() );
						assertEquals( "for case " + signature, correctResult.longValue(), actualResult.longValue() );
						break;
					}
				}
				
				else
				{
					if ( null != correctResult )
					{
						throw new Exception( "for case " + signature, thrown );
					}
				}
			}
			
// TODO: test NumericLiteral/RDF translation
			
			mc.close();
		}
		
		private NumericLiteral createNumericLiteral( final String s )
		{
			NumericLiteral l;
			
			if ( s.equals( "error" ) )
			{
				l = null;
			}
			
			else if ( s.equals( "infinity") )
			{
				l = new NumericLiteral( Double.POSITIVE_INFINITY );
			}
			
			else if ( s.contains( "l" ) )
			{
				l = new NumericLiteral( new Long( s.substring( 0, s.length() - 1 ) ).longValue() );
			}
			
			else if ( s.contains( "." ) )
			{
				l = new NumericLiteral( new Double( s ).doubleValue() );
			}
			
			else
			{
				l = new NumericLiteral( new Integer( s ).intValue() );
			}
			
			return l;
		}
	}
	
	public void runTests()
		throws Exception
	{
		testAsynchronous( new ModelConstructorTest() );
		testAsynchronous( new NumericLiteralTest() );
	}
}

// kate: tab-width 4
