package wurfel;

import org.openrdf.repository.Repository;

import wurfel.JLineTest;
import wurfel.test.WurfelTestCase;

public class ContextTest extends WurfelTestCase
{
    private class ContextConstructorTest extends TestRunnable
    {
        public void test()
            throws WurfelException
        {
            Repository repository = JLineTest.createTestRepository();

            Context context = new Context( repository, "Test Context" );

            try
            {
                repository.shutDown();
            }

            catch ( Throwable t )
            {
                throw new WurfelException( t );
            }
        }
    }

    public void runTests()
        throws Throwable
    {
        testAsynchronous( new ContextConstructorTest() );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
