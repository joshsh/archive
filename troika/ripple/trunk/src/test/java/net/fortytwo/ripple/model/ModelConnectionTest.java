package net.fortytwo.ripple.model;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import java.net.URL;

import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;

import net.fortytwo.ripple.test.RippleTestCase;

public class ModelConnectionTest extends RippleTestCase
{
	private class CreateUriTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = new ModelConnection( getTestModel() );

			URI uri;
			String localName, namespace;

			// Hash namespaces.

			uri = mc.createUri( "http://example.org/foo#bar" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "bar" );
			assertEquals( namespace, "http://example.org/foo#" );

			uri = mc.createUri( "http://example.org/foo#" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "" );
			assertEquals( namespace, "http://example.org/foo#" );

			uri = mc.createUri( "http://example.org/ns/foo/#bar" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "bar" );
			assertEquals( namespace, "http://example.org/ns/foo/#" );

			uri = mc.createUri( "http://example.org/ns/foo/#" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "" );
			assertEquals( namespace, "http://example.org/ns/foo/#" );

			// Slash namespaces.

			uri = mc.createUri( "http://example.org/ns/foo/bar" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "bar" );
			assertEquals( namespace, "http://example.org/ns/foo/" );

			uri = mc.createUri( "http://example.org/ns/foo/" );
			localName = uri.getLocalName();
			namespace = uri.getNamespace();
			assertEquals( localName, "" );
			assertEquals( namespace, "http://example.org/ns/foo/" );

			mc.close();
		}
	}

	private class CountStatementsTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = new ModelConnection( getTestModel() );

			URI context = mc.createUri( "urn:org.example.test.countStatementsTest#" );

			URI uri1 = mc.createUri( "urn:org.example.test#uri1" );
			URI uri2 = mc.createUri( "urn:org.example.test#uri2" );
			URI uri3 = mc.createUri( "urn:org.example.test#uri3" );

			URI [] uris = {uri1, uri2, uri3};
			for ( int i = 0; i < 3; i++ )
				for ( int j = 0; j < 3; j++ )
					for ( int k = 0; k < 3; k++ )
						mc.add(
				new RdfValue( uris[i] ),
				new RdfValue( uris[j] ),
				new RdfValue( uris[k] ), context );

			long count = mc.countStatements( context );
			assertEquals( count, 27 );

			mc.close();
		}
	}

	private class AddStatementsTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = new ModelConnection( getTestModel() );

			URI ctxA = mc.createUri( "urn:org.example.test.addStatementsTest.ctxA#" );
			URI uri1 = mc.createUri( "urn:org.example.test.addStatementsTest.ctxA#uri1" );
			assertEquals( mc.countStatements( ctxA ), 0 );
			mc.add(
		new RdfValue( uri1 ),
		new RdfValue( uri1 ),
		new RdfValue( uri1 ) );
			assertEquals( mc.countStatements( ctxA ), 1 );
		}
	}

	private class AddGraphTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = new ModelConnection( getTestModel() );

			{
				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.turtleStrA#" );

				String s = "@prefix foo:  <http://example.org/foo#>.\n"
					+ "foo:a foo:b foo:c." ;
				InputStream is = new ByteArrayInputStream( s.getBytes() );

				mc.addGraph( is, ctxA, RDFFormat.TURTLE );

				//assertEquals( mc.countStatements( null ), 1 );
				assertEquals( mc.countStatements( ctxA ), 1 );
			}

			{
				URL test1Url = ModelConnectionTest.class.getResource( "test1.ttl" );
				URL test2Url = ModelConnectionTest.class.getResource( "test2.ttl" );

				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.turtleA#" );
				URI ctxB = mc.createUri( "urn:org.example.test.addGraphTest.turtleB#" );

				mc.addGraph( test1Url, ctxA );
				assertEquals( mc.countStatements( ctxA ), 2 );
				mc.addGraph( test2Url, ctxA );
				assertEquals( mc.countStatements( ctxA ), 4 );

				mc.addGraph( test1Url, ctxB );
				assertEquals( mc.countStatements( ctxB ), 2 );
				mc.addGraph( test2Url, ctxB );
				assertEquals( mc.countStatements( ctxB ), 4 );
			}

			{
				URL test1Url = ModelConnectionTest.class.getResource( "test1.rdf" );
				URL test2Url = ModelConnectionTest.class.getResource( "test2.rdf" );

				URI ctxA = mc.createUri( "urn:org.example.test.addGraphTest.rdfxmlA#" );
				URI ctxB = mc.createUri( "urn:org.example.test.addGraphTest.rdfxmlB#" );

				mc.addGraph( test1Url, ctxA );
				assertEquals( mc.countStatements( ctxA ), 2 );
				mc.addGraph( test2Url, ctxA );
				assertEquals( mc.countStatements( ctxA ), 4 );

				mc.addGraph( test1Url, ctxB );
				assertEquals( mc.countStatements( ctxB ), 2 );
				mc.addGraph( test2Url, ctxB );
				assertEquals( mc.countStatements( ctxB ), 4 );
			}
//*/

			mc.close();
		}
	}

	public void runTests()
		throws Throwable
	{
// Note: Sesame repositories have not responded well to asynchronous testing, so ModelConnection will have to add its own synchronization code (synchronize on the Repository).
		testSynchronous( new CreateUriTest() );
		testSynchronous( new CountStatementsTest() );

/* RESTORE ME:
See: http://www.openrdf.org/issues/browse/SES-358?watch=true
net.fortytwo.ripple.model.SesameTest$AddFromInputStreamTest
		testSynchronous( new AddGraphTest() );
*/
	}
}

// kate: tab-width 4
