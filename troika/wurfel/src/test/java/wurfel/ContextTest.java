package wurfel;

import wurfel.test.WurfelTestCase;

public class ContextTest extends WurfelTestCase
{
    private class ContextConstructorTest extends TestRunnable
    {
        public void test()
            throws WurfelException
        {
            Context context = new Context( "Test Context" );
        }
    }

    public void runTests()
        throws Throwable
    {
        testAsynchronous( new ContextConstructorTest() );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
