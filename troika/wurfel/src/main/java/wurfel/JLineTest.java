package wurfel;

import org.openrdf.sesame.Sesame;
import org.openrdf.sesame.repository.local.LocalService;
import org.openrdf.sesame.repository.local.LocalRepository;
import org.openrdf.sesame.config.ConfigurationException;
import org.openrdf.sesame.admin.AdminListener;
import org.openrdf.sesame.admin.StdOutAdminListener;
//import org.openrdf.rio.RDFFormat;

import java.net.URL;

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
        }
/*
        URL myRDFData = new URL( "http://xmlns.com/foaf/0.1/index.rdf" );
        String baseURI = "http://foaf#";
        boolean verifyData = true;
        AdminListener myListener = new StdOutAdminListener();

        myRepository.addData( myRDFData, baseURI, RDFFormat.RDFXML, verifyData, myListener );
*/
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
