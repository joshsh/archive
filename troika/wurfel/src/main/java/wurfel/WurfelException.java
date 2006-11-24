package wurfel;

public class WurfelException extends Exception
{
    public WurfelException( Throwable t )
    {
        t.printStackTrace( System.err );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
