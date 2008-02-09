package net.fortytwo.ripple.model;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import org.openrdf.model.vocabulary.RDF;
import org.openrdf.rio.RDFFormat;

import net.fortytwo.ripple.io.RdfImporter;
import net.fortytwo.ripple.libs.stack.Dup;
import net.fortytwo.ripple.query.ListDequotation;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;

public class OperatorTest extends RippleTestCase
{
	private static final String TEST_1 =
  "@prefix : <urn:test.CreateOperatorTest#>.\n"
+ "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>.\n"
+ "\n"
+ ":simpleList a rdf:List;\n"
+ "    rdf:first \"1\";\n"
+ "    rdf:rest [\n"
+ "        rdf:first \"2\";\n"
+ "        rdf:rest rdf:nil ];\n"
+ "    .\n"
+ ":firstBranchingList a rdf:List;\n"
+ "rdf:first \"1a\";\n"
+ "rdf:first \"1b\";\n"
+ "rdf:rest [\n"
+ "		rdf:first \"2\";\n"
+ "		rdf:rest rdf:nil ];\n"
+ ".";
	
	private class CreateOperatorTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
			ModelConnection mc = model.getConnection( "for CreateOperatorTest" );
			
			InputStream is = new ByteArrayInputStream( TEST_1.getBytes() );
			RdfImporter importer = new RdfImporter( mc );
			SesameInputAdapter sc = new SesameInputAdapter( importer );
			RdfUtils.read( is, sc, "", RDFFormat.TURTLE );
			mc.commit();
			is.close();
			
			Collector<Operator> ops = new Collector<Operator>();
			RippleValue arg;
			RippleValue a1 = mc.value( "1" );
			RippleValue a2 = mc.value( "2" );
			
			// a RippleList --> ListDequotation
			arg = mc.list( a2 ).push( a1 );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof ListDequotation );
			
			// the nil RippleList --> ListDequotation
			arg = RippleList.NIL;
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof ListDequotation );
			
			// rdf:nil --> ListDequotation
			arg = new RdfValue( RDF.NIL );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof ListDequotation );
			
			// an rdf:List --> ListDequotation
			arg = new RdfValue( mc.createUri( "urn:test.CreateOperatorTest#simpleList" ) );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof ListDequotation );
			
			// a branching rdf:List --> multiple ListDequotations
			arg = new RdfValue( mc.createUri( "urn:test.CreateOperatorTest#firstBranchingList" ) );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 2, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof ListDequotation );
			
			// a PrimitiveStackRelation --> the same PrimitiveStackRelation
			arg = new Dup();
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof Dup );
			
			// an rdf:Property --> RdfPredicateRelation
			arg = new RdfValue( RDF.TYPE );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof RdfPredicateRelation );

			// a non-property RdfValue which is not anything else --> RdfPredicateRelation
			arg = new RdfValue( RDF.BAG );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof RdfPredicateRelation );
			
			// anything else --> NullFilter
			arg = mc.value( 42 );
			ops.clear();
			Operator.createOperator( arg, ops, mc );
			assertEquals( 1, ops.size() );
			assertTrue( ops.iterator().next().getRelation() instanceof NullFilter );
			
			mc.close();
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new CreateOperatorTest() );
	}
}
