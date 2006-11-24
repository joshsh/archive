package wurfel;

import org.openrdf.sesame.Sesame;
import org.openrdf.sesame.repository.local.LocalService;
import org.openrdf.sesame.repository.local.LocalRepository;
import org.openrdf.sesame.config.ConfigurationException;
import org.openrdf.sesame.admin.AdminListener;
import org.openrdf.sesame.admin.StdOutAdminListener;
import org.openrdf.sesame.constants.RDFFormat;
import org.openrdf.sesame.config.AccessDeniedException;

import java.net.URL;
import java.net.MalformedURLException;

import java.io.IOException;

public class JLineTest
{
    private static void testSesame()
    {
        LocalService service = Sesame.getService();
        boolean inferencing = true;
        LocalRepository myRepository;

        try
        {
             myRepository = service.createRepository( "myRep", inferencing );
        }

        catch ( ConfigurationException e )
        {
            System.err.println( e.getMessage() );
            return;
        }

        URL myRDFData;

        try
        {
            myRDFData = new URL( "http://xmlns.com/foaf/0.1/index.rdf" );
        }

        catch ( MalformedURLException e )
        {
            System.err.println( e.getMessage() );
            return;
        }

        String baseURI = "http://foaf#";
        boolean verifyData = true;
        AdminListener myListener = new StdOutAdminListener();

        try
        {
            myRepository.addData( myRDFData, baseURI, RDFFormat.RDFXML, verifyData, myListener );
        }

        catch ( IOException e )
        {
            System.err.println( e.getMessage() );
            return;
        }

        catch ( AccessDeniedException e )
        {
            System.err.println( e.getMessage() );
            return;
        }
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


        testSesame();




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
