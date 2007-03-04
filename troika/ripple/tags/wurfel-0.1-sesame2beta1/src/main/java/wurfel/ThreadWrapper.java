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

        new Thread( new Runnable() {
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
        } ).start();

        try
        {
            synchronized( monitorObj )
            {
                if ( timeout > 0 )
                    monitorObj.wait( timeout );
                else
                    monitorObj.wait();
            }
        }

        catch ( InterruptedException e )
        {
            throw new WurfelException( e );
        }

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
