package wurfel;

import wurfel.model.Model;
import wurfel.model.ModelMock;
import wurfel.model.Apply;
import wurfel.model.NodeSet;

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

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.impl.LiteralImpl;

import java.io.IOException;

import java.net.URL;

import java.util.Iterator;
import java.util.Set;
import java.util.Collection;
import java.util.List;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.LinkedHashSet;

import org.apache.log4j.Logger;

public class Context
{
    private final static Logger s_logger = Logger.getLogger( Context.class );

    private final static boolean s_useInferencing = true;

    private static final URI
        // TODO: this URI doesn't exist in the wurfel model.
        s_wurfelIdentifierUri = Wurfel.getWurfelUri( "Identifier" ),
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" ),
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" ),
        s_rdfFirstUri = Wurfel.getRdfUri( "first" ),
        s_rdfRestUri = Wurfel.getRdfUri( "rest" ),
        s_rdfNilUri = Wurfel.getRdfUri( "nil" );

    ////////////////////////////////////////////////////////////////////////////

    public Value getValue( Value arg, Value func )
        throws WurfelException
    {
        Collection<Value> results = model.multiply( arg, func );

        if ( 1 != results.size() )
        {
            if ( 0 == results.size() )
                throw new WurfelException( "no values resolved for " + func.toString() + " of " + arg.toString() );
            else
                throw new WurfelException( func.toString() + " of " + arg.toString() + " resolved to more than one value" );
        }

        else
            return results.iterator().next();
    }

    public Resource getResource( Value arg, Value func )
        throws WurfelException
    {
        Value val = getValue( arg, func );

        if ( val instanceof Resource )
            return (Resource) val;

        else
            throw new WurfelException( "value " + val.toString() + " is not a Resource" );
    }

    public URI getUri( Value arg, Value func )
        throws WurfelException
    {
        Value val = getValue( arg, func );

        if ( val instanceof URI )
            return (URI) val;

        else
            throw new WurfelException( "value " + val.toString() + " is not a URI" );
    }

    public Literal getLiteral( Value arg, Value func )
        throws WurfelException
    {
        Value val = getValue( arg, func );

        if ( val instanceof Literal )
            return (Literal) val;

        else
            throw new WurfelException( "value " + val.toString() + " is not a Literal" );
    }

    public String getString( Value arg, Value func )
        throws WurfelException
    {
        Literal lit = getLiteral( arg, func );

        URI type = lit.getDatatype();
        if ( !type.equals( s_xsdStringUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdStringUri.toString() + ", found " + type.toString() );

        String label = lit.getLabel();
        return label;
    }

    public boolean getBoolean( Value arg, Value func )
        throws WurfelException
    {
        Literal lit = getLiteral( arg, func );

        URI type = lit.getDatatype();
        if ( !type.equals( s_xsdBooleanUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdBooleanUri.toString() + ", found " + type.toString() );

        String label = lit.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
        return label.equals( "true" );
    }

    public List<Resource> getRdfList( final Resource listHead )
        throws WurfelException
    {
        List<Resource> list = new ArrayList<Resource>();

        Resource cur = getResource( listHead, s_rdfFirstUri );

// TODO: is this 'equals' safe?
        while ( !cur.equals( s_rdfNilUri ) )
        {
            list.add( cur );
            cur = getResource( cur, s_rdfRestUri );
        }

        return list;
    }

    ////////////////////////////////////////////////////////////////////////////

    String name;
    LocalRepository repository;
    Collection<URL> importedDataURLs;
Hashtable<String, String> aliases;
Model model = null;

    public Context( final String name )
        throws WurfelException
    {
        s_logger.debug( "Creating new Context '" + name + "'" );

aliases = new Hashtable<String, String>();
        this.name = name;
        importedDataURLs = new ArrayList<URL>();

        LocalService service = Sesame.getService();

        try
        {
             repository = service.createRepository( name + "-repo", s_useInferencing );
        }

        catch ( ConfigurationException e )
        {
            throw new WurfelException( e );
        }

//System.out.println( "Wurfel.schemaUrl() = " + Wurfel.schemaUrl() );
        importModel( Wurfel.schemaUrl(), "urn:wurfel" );
        importModel( Wurfel.testUrl(), "urn:wurfel-test" );
        updateModel();
    }

    public void importModel( final URL url, final String baseURI )
        throws WurfelException
    {
        s_logger.debug( "Importing model " + url.toString() +
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

    public String resolveSimple( String name )
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



/*
    private Node resolveIdentifers( Node expr )
    {
        if ( expr instanceof 
    }
*/

    public Literal newIdentifier( String s )
    {
        return new LiteralImpl( s, s_wurfelIdentifierUri );
    }

    public Literal newStringLiteral( final String s )
    {
        return new LiteralImpl( s, s_xsdStringUri );
    }

    private boolean isIdentifier( Value v )
    {
        if ( v instanceof Literal )
            return ( ( (Literal) v ).getDatatype() ).equals( s_wurfelIdentifierUri );
        else
            return false;
    }

    private boolean isApply( Value v )
    {
        return v instanceof Apply;
    }

    private boolean isCombinator( Value v )
    {
return false;
    }

    // Note: this should throw an exception if the identifier does not resolve
    //       to a unique value.
    private Value resolve( String s )
        throws WurfelException
    {
        return model.resolve( s );
    }

    public Value resolveIdentifiers( Value expr )
        throws WurfelException
    {
        if ( isIdentifier( expr ) )
            return resolve( ( (Literal) expr ).getLabel() );

        else if ( isApply( expr ) )
        {
            Value function = resolveIdentifiers( ( (Apply) expr ).getFunction() );
            Value argument = resolveIdentifiers( ( (Apply) expr ).getArgument() );

            if ( null == function || null == argument )
                return null;
            else
                return new Apply( function, argument );
        }

        else
            return expr;
    }

    public Set<Value> apply( Value func, Value arg )
        throws WurfelException
    {
// TODO: combinators and primitives as func will have their own, idiosyncratic ways of yielding a product
        return model.multiply( arg, func );
    }

    public NodeSet reduce( Value expr )
        throws WurfelException
    {
        if ( isApply( expr ) )
        {
            Iterator<Value> reducedFuncIter = reduce(
                ( (Apply) expr ).getFunction() ).iterator();

            Iterator<Value> reducedArgIter = reduce(
                ( (Apply) expr ).getArgument() ).iterator();

            NodeSet result = new NodeSet();
            while ( reducedFuncIter.hasNext() )
            {
                Value function = reducedFuncIter.next();
                while ( reducedArgIter.hasNext() )
                    result.add(
                        apply( function, reducedArgIter.next() ) );
            }

            return result;
        }

        else
            return new NodeSet( expr );
    }

    public Set<Value> evaluate( Value expr )
        throws WurfelException
    {
        Value resolved = resolveIdentifiers( expr );
        return ( null == resolved )
            ? new NodeSet()
            : reduce( resolved );
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
