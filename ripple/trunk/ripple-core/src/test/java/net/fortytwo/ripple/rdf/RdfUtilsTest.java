package net.fortytwo.ripple.rdf;

import java.io.InputStream;

import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;
import org.openrdf.rio.RDFFormat;

public class RdfUtilsTest extends RippleTestCase
{
	private class ReadFromInputStreamTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for ReadFromInputStreamTest" );

			InputStream is = RdfUtilsTest.class.getResourceAsStream( "rdfUtilsReadTest.ttl" );
			RdfCollector allRdf = new RdfCollector();
			SesameInputAdapter sc = new SesameInputAdapter( allRdf );
			RdfUtils.read( is, sc, "", RDFFormat.TURTLE );
			is.close();

			Collector<Statement> allStatements = new Collector<Statement>();
			Collector<Namespace> allNamespaces = new Collector<Namespace>();
			allRdf.statementSource().writeTo( allStatements );
			allRdf.namespaceSource().writeTo( allNamespaces );
			
			assertEquals( 4, allStatements.size() );
			assertEquals( 3, allNamespaces.size() );
				
			mc.close();
		}
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new ReadFromInputStreamTest() );
	}
}

