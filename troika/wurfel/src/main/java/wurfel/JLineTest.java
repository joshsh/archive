package wurfel;

import org.openrdf.model.Graph;
import org.openrdf.model.Value;
import org.openrdf.sesame.Sesame;
import org.openrdf.sesame.admin.AdminListener;
import org.openrdf.sesame.admin.StdOutAdminListener;
import org.openrdf.sesame.config.AccessDeniedException;
import org.openrdf.sesame.config.ConfigurationException;
import org.openrdf.sesame.constants.QueryLanguage;
import org.openrdf.sesame.constants.RDFFormat;
import org.openrdf.sesame.query.MalformedQueryException;
import org.openrdf.sesame.query.QueryResultsTable;
import org.openrdf.sesame.query.QueryEvaluationException;
import org.openrdf.sesame.repository.local.LocalRepository;
import org.openrdf.sesame.repository.local.LocalService;

import org.openrdf.model.Resource;
import org.openrdf.model.Value;
import org.openrdf.model.URI;

import java.util.Iterator;
import java.util.Set;

import java.net.URL;
import java.net.MalformedURLException;

import java.io.IOException;

public class JLineTest
{
    private static void testQuery( LocalRepository myRepository, final String query )
        throws WurfelException
    {
        QueryResultsTable resultsTable;

        try
        {
            resultsTable = myRepository.performTableQuery(QueryLanguage.SERQL, query);
        }

        catch( IOException e )
        {
            throw new WurfelException( e );
        }

        catch( MalformedQueryException e )
        {
            throw new WurfelException( e );
        }

        catch ( QueryEvaluationException e )
        {
            throw new WurfelException( e );
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

        int rowCount = resultsTable.getRowCount();
        int columnCount = resultsTable.getColumnCount();

        for (int row = 0; row < rowCount; row++) {
            for (int column = 0; column < columnCount; column++) {
                Value value = resultsTable.getValue(row, column);

                if (value != null) {
                    System.out.print(value.toString());
                }
                else {
                    System.out.print("null");
                }

                System.out.print("\t");
            }

            System.out.println();
        }
    }

    private static void testModel( Graph myGraph )
        throws WurfelException
    {
        Model model = new Model( myGraph );

        Set<Resource> subjects = model.getSubjects();
        Iterator<Resource> subjIter = subjects.iterator();
        while ( subjIter.hasNext() )
        {
            Resource subject = subjIter.next();
            System.out.println( subject.toString() );

            Set<URI> predicates = model.getPredicates( subject );
            Iterator<URI> predIter = predicates.iterator();
            while ( predIter.hasNext() )
            {
                URI predicate = predIter.next();
                System.out.println( "    " + predicate.toString() );

                Set<Value> objects = model.multiply( subject, predicate );
                Iterator<Value> objIter = objects.iterator();
                while ( objIter.hasNext() )
                    System.out.println( "        " + objIter.next().toString() );
            }
        }
    }

    private static void testGraph( LocalRepository myRepository )
        throws WurfelException
    {
        Graph myGraph;

        try
        {
            myGraph = myRepository.getGraph();
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

        testModel( myGraph );
    }

    private static void testSesame()
        throws WurfelException
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
            throw new WurfelException( e );
        }

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

        String baseURI = "http://ns0";
        boolean verifyData = true;
        AdminListener myListener = new StdOutAdminListener();

        try
        {
            myRepository.addData( myRDFData, baseURI, RDFFormat.RDFXML, verifyData, myListener );
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

//        testQuery( myRepository, "SELECT * FROM {x} p {y}" );
        testGraph( myRepository );
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
            testSesame();
        }

        catch ( WurfelException e )
        {
            //...
        }



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
