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

import java.io.IOException;

import java.net.URL;

import java.util.Iterator;
import java.util.Set;
import java.util.Collection;
import java.util.ArrayList;
import java.util.Hashtable;

import org.apache.log4j.Logger;

public class Context
{
    final static Logger logger = Logger.getLogger( Context.class );

    String name;
    LocalRepository repository;
    Collection<URL> importedDataURLs;
Hashtable<String, String> aliases;
Model model = null;

    public Context( final String name )
        throws WurfelException
    {
        logger.debug( "Creating new Context '" + name + "'" );

aliases = new Hashtable<String, String>();
        this.name = name;
        importedDataURLs = new ArrayList<URL>();

        LocalService service = Sesame.getService();
        boolean inferencing = true;

        try
        {
             repository = service.createRepository( name + "-repo", inferencing );
        }

        catch ( ConfigurationException e )
        {
            throw new WurfelException( e );
        }

System.out.println( "Wurfel.schemaUrl() = " + Wurfel.schemaUrl() );
        importModel( Wurfel.schemaUrl(), "urn:wurfel" );
        updateModel();
    }

    public void importModel( final URL url, final String baseURI )
        throws WurfelException
    {
        logger.debug( "Importing model " + url.toString() +
            ( ( null == baseURI ) ? "" : " as " + baseURI ) );

        boolean verifyData = true;
        AdminListener myListener = new StdOutAdminListener();

        try
        {
            repository.addData( url, baseURI, RDFFormat.RDFXML, verifyData, myListener );
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

        importedDataURLs.add( url );
        updateModel();
    }

    // TODO -- See: http://openrdf.org/issues/browse/SES-304
    public int countStatements()
    {
        return 0;
    }






    public void define( String name, String uri )
    {
        aliases.put( name, uri );
    }

    public void show( String name )
        throws WurfelException
    {
        String s = aliases.get( name );

        if ( null != s )
            System.out.println( s );

        else if ( null != model )
        {
            URI uri = model.resolve( name );
            if ( null != uri )
                show( uri );
            else
                System.out.println( "()" );
        }

        else
            System.out.println( "()" );
    }

    private void show( Resource subject )
        throws WurfelException
    {
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

    public String resolve( String name )
    {
        String s = aliases.get( name );
        if ( null != s )
            return s;
        else if ( null != model )
        {
            URI uri = model.resolve( name );
            if ( null != uri )
                return uri.toString();
        }

        return null;
    }








    private void updateModel()
        throws WurfelException
    {
        Graph myGraph;

        try
        {
            myGraph = repository.getGraph();
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

        model = new ModelMock( myGraph );
    }



    //FIXME: temporary method
    public Model getModel()
    {
        return model;
    }

    //FIXME: temporary method
    public void testQuery( final String query )
        throws WurfelException
    {
        QueryResultsTable resultsTable;

        try
        {
            resultsTable = repository.performTableQuery( QueryLanguage.SERQL, query );
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

    private void testModel()
        throws WurfelException
    {
        Set<Resource> subjects = model.getSubjects();
        Iterator<Resource> subjIter = subjects.iterator();
        while ( subjIter.hasNext() )
        {
            Resource subject = subjIter.next();
            show( subject );
        }
    }

    public void testGraph()
        throws WurfelException
    {
        updateModel();

        testModel();
    }

    public void printStatements()
        throws WurfelException
    {
        testGraph();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
