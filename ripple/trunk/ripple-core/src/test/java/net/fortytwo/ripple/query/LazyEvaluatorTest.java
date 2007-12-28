package net.fortytwo.ripple.query;

import java.util.Arrays;
import java.util.Iterator;

import net.fortytwo.ripple.libs.math.Abs;
import net.fortytwo.ripple.libs.math.Sqrt;
import net.fortytwo.ripple.libs.stack.Dup;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.ListMemoizer;

public class LazyEvaluatorTest extends RippleTestCase
{
	public static void assertCollectorsEqual( Collector<RippleList> expected, Collector<RippleList> actual ) throws Exception
	{
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
	
	private class SimpleTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "foo" );
			Evaluator eval = new LazyEvaluator();
			Collector<RippleList> expected = new Collector<RippleList>();
			Collector<RippleList> actual = new Collector<RippleList>();
			RippleList input;
			
			RippleValue op = Operator.OP;
			RippleValue
				dup = new Dup(),
				sqrt = new Sqrt(),
				abs = new Abs();
			RippleValue
				minusone = mc.value( -1.0 ),
				one = mc.value( 1 ),
				two = mc.value( 2 );

			// passive stack passes through unchanged
			// (1 2) -> (1 2)
			input = createStack( mc, one, two );
			expected.clear();
			expected.put( createStack( mc, one, two ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// replacement rules are applied at the head of the stack
			// (1 /dup) -> (1 1)
			input = createStack( mc, one, dup, op );
			expected.clear();
			expected.put( createStack( mc, one, one ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// evaluation is recursive
			// (1 /dup /dup) -> (1 1 1)
			input = createStack( mc, one, dup, op, dup, op );
			expected.clear();
			expected.put( createStack( mc, one, one, one ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// evaluator drops anything which can't be reduced to head-normal form
			// (/dup) ->
			input = createStack( mc, dup, op );
			expected.clear();
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// evaluator drops the nil list
			// () ->
			input = RippleList.NIL;
			expected.clear();
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// distributive reduction
			// (1 /sqrt /dup) -> (1 1), (-1, -1)
			input = createStack( mc, one, sqrt, op, dup, op );
			expected.clear();
			expected.put( createStack( mc, one, one ) );
			expected.put( createStack( mc, minusone, minusone ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// no eager reduction
			// (2 /dup 1) -> (2 /dup 1)
			input = createStack( mc, two, dup, op, one );
			expected.clear();
			expected.put( createStack( mc, two, dup, op, one ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// lists are opaque
			// ((2 /dup)) -> ((2 /dup))
			input = createStack( mc, createQueue( mc, two, dup, op ) );
			expected.clear();
			expected.put( createStack( mc, createQueue( mc, two, dup, op ) ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// list dequotation
			// (2 /(1 /dup)) -> (2 1 1)
			input = createStack( mc, two, createQueue( mc, one, dup, op ), op );
			expected.clear();
			expected.put( createStack( mc, two, one, one ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			// results are not necessarily a set
			// (1 /sqrt /abs)
			input = createStack( mc, one, sqrt, op, abs, op );
			expected.clear();
			expected.put( createStack( mc, one ) );
			expected.put( createStack( mc, one ) );
			actual.clear();
			eval.applyTo( input, actual, mc );
			assertCollectorsEqual( expected, actual );
			
			mc.close();
		}
	}
	
	public void runTests()
		throws Exception
	{
		testAsynchronous( new SimpleTest() );
	}
}
