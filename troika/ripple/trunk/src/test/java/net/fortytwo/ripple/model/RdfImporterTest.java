package net.fortytwo.ripple.model;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import java.net.URL;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSourceAdapter;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.RdfUtils;

import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;

public class RdfImporterTest extends RippleTestCase
{
	private class ImporterTest extends TestRunnable
	{
		void addGraph( final InputStream is,
						final URI context,
						final RDFFormat format,
						final ModelConnection mc )
			throws RippleException
		{
			RdfImporter importer = new RdfImporter( mc, context );
			RdfSourceAdapter adapter = new RdfSourceAdapter( importer );
			RdfUtils.read( is, adapter, context.toString(), format );
		}

		void addGraph( final URL url,
						final URI context,
						final RDFFormat format,
						final ModelConnection mc )
			throws RippleException
		{
			RdfImporter importer = new RdfImporter( mc, context );
			RdfSourceAdapter adapter = new RdfSourceAdapter( importer );
			RdfUtils.read( url, adapter, context.toString(), format );
		}

		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for ImporterTest" );

			{
				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.turtleStrA#" );

				String s = "@prefix foo:  <http://example.org/foo#>.\n"
					+ "foo:a foo:b foo:c." ;
				InputStream is = new ByteArrayInputStream( s.getBytes() );

				addGraph( is, ctxA, RDFFormat.TURTLE, mc );

				//assertEquals( mc.countStatements( null ), 1 );
				assertEquals( mc.countStatements( ctxA ), 1 );
			}

			{
				URL test1Url = ModelConnectionTest.class.getResource( "test1.ttl" );
				URL test2Url = ModelConnectionTest.class.getResource( "test2.ttl" );

				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.turtleA#" );
				URI ctxB = mc.createUri( "urn:org.example.test.addGraphTest.turtleB#" );

				addGraph( test1Url, ctxA, RDFFormat.TURTLE, mc );
				assertEquals( mc.countStatements( ctxA ), 2 );
				addGraph( test2Url, ctxA, RDFFormat.TURTLE, mc );
				assertEquals( mc.countStatements( ctxA ), 4 );

				addGraph( test1Url, ctxB, RDFFormat.TURTLE, mc );
				assertEquals( mc.countStatements( ctxB ), 2 );
				addGraph( test2Url, ctxB, RDFFormat.TURTLE, mc );
				assertEquals( mc.countStatements( ctxB ), 4 );
			}

			{
				URL test1Url = ModelConnectionTest.class.getResource( "test1.rdf" );
				URL test2Url = ModelConnectionTest.class.getResource( "test2.rdf" );

				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.rdfxmlA#" );
				URI ctxB = mc.createUri( "urn:org.example.test.addGraphTest.rdfxmlB#" );

				addGraph( test1Url, ctxA, RDFFormat.RDFXML, mc );
				assertEquals( mc.countStatements( ctxA ), 2 );
				addGraph( test2Url, ctxA, RDFFormat.RDFXML, mc );
				assertEquals( mc.countStatements( ctxA ), 4 );

				addGraph( test1Url, ctxB, RDFFormat.RDFXML, mc );
				assertEquals( mc.countStatements( ctxB ), 2 );
				addGraph( test2Url, ctxB, RDFFormat.RDFXML, mc );
				assertEquals( mc.countStatements( ctxB ), 4 );
			}

			mc.close();
		}
	}

	public void runTests()
		throws Exception
	{
//See: http://www.openrdf.org/issues/browse/SES-358?watch=true
		testSynchronous( new ImporterTest() );
	}
}

// kate: tab-width 4
