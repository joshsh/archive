package wurfel.model;

import org.openrdf.model.URI;

import wurfel.test.WurfelTestCase;

public class ModelConnectionTest extends WurfelTestCase
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

    public void runTests()
        throws Throwable
    {
        testAsynchronous( new CreateUriTest() );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
