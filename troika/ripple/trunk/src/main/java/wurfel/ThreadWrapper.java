package wurfel;

public abstract class ThreadWrapper
{
    protected abstract void run() throws WurfelException;

    private boolean finished;
    private WurfelException error;

    public void start( final long timeout )
        throws WurfelException
    {
        finished = false;
        error = null;

        final Object monitorObj = "";
        final ThreadWrapper tw = this;

        Thread t = new Thread( new Runnable() {
            public void run()
            {
                try
                {
                    tw.run();
                }

                catch ( WurfelException e )
                {
                    error = e;
                }

                finished = true;
                synchronized( monitorObj )
                {
                    monitorObj.notify();
                }
            }
        } );

        t.start();

        try
        {
            synchronized( monitorObj )
            {
                if ( timeout > 0 )
{
System.out.println( "wait(" + timeout + ")" );
                    monitorObj.wait( timeout );
}
                else
{
                    monitorObj.wait();
System.out.println( "wait()" );
}
System.out.println( "done! #####################" ); System.out.flush();
            }

            if ( !finished )
                t.interrupt();
        }

        catch ( InterruptedException e )
        {
System.out.println( "catch ( InterruptedException e )" ); System.out.flush();
            throw new WurfelException( e );
        }
System.out.println( "finished = " + finished );

        if ( !finished )
            throw new WurfelException( "operation timed out" );

        else if ( null != error )
            throw error;
    }

    public void start()
        throws WurfelException
    {
        start( -1 );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
