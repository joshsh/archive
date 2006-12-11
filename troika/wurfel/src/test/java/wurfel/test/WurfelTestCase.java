package wurfel.test;

import wurfel.Wurfel;
import wurfel.WurfelException;

import junit.framework.TestCase;
import junit.framework.AssertionFailedError;

public abstract class WurfelTestCase extends TestCase
{
    protected static final boolean DEBUG = true;

    private int totalRunningTests;
    private Throwable error = null;

    ////////////////////////////////////////////////////////////////////////////

    protected abstract class TestRunnable implements Runnable
    {
        public abstract void test()
            throws WurfelException;

        public void run()
        {
            if ( DEBUG )
                System.out.println( this.getClass().getName() );

            try
            {
                test();
            }

            catch ( Throwable t )
            {
                error = t;
            }

            finishTest();
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    private void startTest()
    {
        totalRunningTests++;
    }

    private synchronized void finishTest()
    {
        totalRunningTests--;

        if ( 0 == totalRunningTests )
            notify();
    }

    protected void testSynchronous( TestRunnable r )
    {
        startTest();

        r.run();
    }

    protected void testAsynchronous( TestRunnable r )
    {
        startTest();

        ( new Thread( r ) ).start();
    }

    protected void start()
    {
        Wurfel.initialize();

        totalRunningTests = 0;
    }

    protected synchronized void finish()
        throws Throwable
    {
        try
        {
            while ( 0 < totalRunningTests )
            {
                wait();
                if ( null != error )
                    break;
            }
        }

        catch ( InterruptedException e )
        {
            e.printStackTrace( System.err );
            fail( e.toString() );
        }

        if ( null != error )
            throw error;
    }

    public abstract void runTests()
        throws Throwable;

    public void testAll()
        throws Throwable
    {
        start();

        runTests();

        finish();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
