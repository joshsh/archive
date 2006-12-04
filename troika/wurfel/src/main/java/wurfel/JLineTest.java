package wurfel;

import wurfel.cli.Interpreter;

import java.net.URL;
import java.net.MalformedURLException;

import java.io.IOException;

public class JLineTest
{
    private static void testSesame()
        throws WurfelException
    {
        Context context = new Context( "myContext" );
        URL myRDFData;

        try
        {
            myRDFData = new URL( "http://www.kanzaki.com/ns/topic.rdf" );
//            myRDFData = new URL( "http://xmlns.com/foaf/0.1/index.rdf" );
        }

        catch ( MalformedURLException e )
        {
            throw new WurfelException( e );
        }

        context.add( myRDFData, "http://tempuri.org" );

//        context.testQuery( myRepository, "SELECT * FROM {x} p {y}" );
        context.testGraph();
    }

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
            Interpreter r = new Interpreter();
            r.run();
        }

        catch ( Exception e )
        {
            System.out.println( e.toString() );
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
