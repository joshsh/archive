package wurfel;


public class JLineTest
{
    public static void main( final String [] args )
    {
/*
        List<String> candidates = singleInstance.complete( args[0], 0 );

        Iterator<String> iter = candidates.iterator();
        while ( iter.hasNext() )
        {
            String s = iter.next();
            System.out.println( s );
        }
*/

        try
        {
            ParserRunnable r = new ParserRunnable();
            r.run();
        }

        catch ( Exception e )
        {
            System.out.println( e.toString() );
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
