package net.fortytwo.ripple.query;

import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;

public class QueryPipeTest extends RippleTestCase
{
	private class QueriesTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = RippleTestCase.getTestModel();
			Evaluator eval = new LazyEvaluator();
			QueryEngine qe = new QueryEngine( model, eval, System.out, System.err );
			Collector<RippleList> expected = new Collector<RippleList>();
			Collector<RippleList> results = new Collector<RippleList>();
			QueryPipe qp = new QueryPipe( qe, results );
			ModelConnection mc = qe.getConnection();
			
			RippleValue
					zero = mc.value( 0 ),
					four = mc.value( 4 ),
					five = mc.value( 5 );
			
			// A simple expression.
			results.clear();
			qp.put( "2 3 /add.\n" );
			expected.clear();
			expected.put( LazyEvaluatorTest.createStack( mc, five ) );
			LazyEvaluatorTest.assertCollectorsEqual( expected, results );

			// A slightly more complex expression.
			results.clear();
			qp.put( "105"
					+ " ((1 2 3 4 5) 0 add /fold) 7/times"
					+ " add 6/times /sub.\n" );
			expected.clear();
			expected.put( LazyEvaluatorTest.createStack( mc, zero ) );
			LazyEvaluatorTest.assertCollectorsEqual( expected, results );
			
			// A branching expression.
			results.clear();
			qp.put( "(1 2)/each 3 /add.\n" );
			expected.clear();
			expected.put( LazyEvaluatorTest.createStack( mc, four ) );
			expected.put( LazyEvaluatorTest.createStack( mc, five ) );
			LazyEvaluatorTest.assertCollectorsEqual( expected, results );
			
			qp.close();
			mc.close();
		}
	}
	
	public void runTests()
		throws Exception
	{
		testAsynchronous( new QueriesTest() );
	}
}
