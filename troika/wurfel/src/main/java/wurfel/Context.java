package wurfel;

import wurfel.model.Model;
import wurfel.model.ModelMock;
import wurfel.model.Apply;
import wurfel.model.NodeSet;
import wurfel.model.Function;
import wurfel.model.primitives.IntegerAdd;
import wurfel.model.primitives.IntegerSubtract;
import wurfel.model.primitives.IntegerMultiply;
import wurfel.model.primitives.IntegerDivide;
import wurfel.model.primitives.IntegerMod;
import wurfel.model.primitives.ConcatenateStringsPrimitive;
import wurfel.model.primitives.misc.Sha1SumOf;
import wurfel.model.primitives.misc.UrlEncoding;
import wurfel.model.primitives.misc.UrlTarget;
import wurfel.model.combinators.Combinator_B;
import wurfel.model.combinators.Combinator_C;
import wurfel.model.combinators.Combinator_I;
import wurfel.model.combinators.Combinator_K;
import wurfel.model.combinators.Combinator_L;
import wurfel.model.combinators.Combinator_O;
import wurfel.model.combinators.Combinator_R;
import wurfel.model.combinators.Combinator_S;
import wurfel.model.combinators.Combinator_T;
import wurfel.model.combinators.Combinator_U;
import wurfel.model.combinators.Combinator_V;
import wurfel.model.combinators.Combinator_W;
import wurfel.model.combinators.Combinator_Y;
import wurfel.model.combinators.Combinator_w;

import org.openrdf.model.Graph;
import org.openrdf.model.Value;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.rio.rdfxml.RdfXmlWriter;
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

import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.net.URL;

import java.util.Iterator;
import java.util.Set;
import java.util.Collection;
import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Hashtable;
import java.util.LinkedHashSet;

import org.apache.log4j.Logger;

public class Context
{
    private final static Logger s_logger = Logger.getLogger( Context.class );
    private static final AdminListener s_adminListener
        = new StdOutAdminListener();


    private final static boolean s_useInferencing = true;

// TODO: determine whether these URIs should really be created via the model's
//       graph's ValueFactory.
    private static final URI
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" ),
        s_xsdDoubleUri = Wurfel.getXmlSchemaUri( "double" ),
        s_xsdIntegerUri = Wurfel.getXmlSchemaUri( "integer" ),
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" ),
        s_rdfFirstUri = Wurfel.getRdfUri( "first" ),
        s_rdfRestUri = Wurfel.getRdfUri( "rest" ),
        s_rdfNilUri = Wurfel.getRdfUri( "nil" );

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func )
        throws WurfelException
    {
        Apply a = new Apply( func, arg );
        return reduce( a );
    }

    public Value findUniqueProduct( Value arg, Value func )
        throws WurfelException
    {
        Collection<Value> results = findProduct( arg, func );

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

    public Resource castToResource( Value v )
        throws WurfelException
    {
        if ( v instanceof Resource )
            return (Resource) v;
        else
            throw new WurfelException( "value " + v.toString() + " is not a Resource" );
    }

    public URI castToUri( Value v )
        throws WurfelException
    {
        if ( v instanceof URI )
            return (URI) v;

        else
            throw new WurfelException( "value " + v.toString() + " is not a URI" );
    }

    public Literal castToLiteral( Value v )
        throws WurfelException
    {
        if ( v instanceof Literal )
            return (Literal) v;

        else
            throw new WurfelException( "value " + v.toString() + " is not a Literal" );
    }

    public boolean booleanValue( Literal l )
        throws WurfelException
    {
/*
        URI type = lit.getDatatype();
        if ( !type.equals( s_xsdBooleanUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdBooleanUri.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
// TODO: is capitalization relevant? Can 'true' also be represented as '1'?
        return label.equals( "true" );
    }

    public int intValue( Literal l )
        throws WurfelException
    {
/*
        URI type = l.getDatatype();
        if ( !type.equals( s_xsdIntegerUri ) )
            throw new WurfelException( "type mismatch: expected " + s_xsdIntegerUri.toString() + ", found " + type.toString() );
*/

        String label = l.getLabel();
        try
        {
            return ( new Integer( label ) ).intValue();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public String stringValue( Literal l )
        throws WurfelException
    {
        return l.getLabel();
    }

    public List<Value> listValue( final Resource listHead )
        throws WurfelException
    {
        List<Value> list = new ArrayList<Value>();

        Resource cur = listHead;

// TODO: is this 'equals' safe?
        while ( !cur.equals( s_rdfNilUri ) )
        {
            Value val = findUniqueProduct( cur, s_rdfFirstUri );
            list.add( val );
            cur = castToResource( findUniqueProduct( cur, s_rdfRestUri ) );
        }

        return list;
    }

    ////////////////////////////////////////////////////////////////////////////

    public Literal createLiteral( final String s )
    {
        return model.getValueFactory().createLiteral( s, s_xsdStringUri );
    }

    public Literal createLiteral( final int i )
    {
        return model.getValueFactory().createLiteral( "" + i, s_xsdIntegerUri );
    }

    public Literal createLiteral( final double d )
    {
        return model.getValueFactory().createLiteral( "" + d, s_xsdDoubleUri );
    }

    ////////////////////////////////////////////////////////////////////////////

    String name;
    LocalRepository repository;
    Collection<URL> importedDataURLs;

Hashtable<String, String> aliases;
Model model = null;

    private Hashtable<URI, Function> specialFunctions;

    private void addSpecialFunction( Function f )
    {
        specialFunctions.put( f.getUri(), f );
    }

    private void loadPrimitives()
        throws WurfelException
    {
        specialFunctions = new Hashtable<URI, Function>();

        addSpecialFunction( new IntegerAdd( this ) );
        addSpecialFunction( new IntegerSubtract( this ) );
        addSpecialFunction( new IntegerMultiply( this ) );
        addSpecialFunction( new IntegerDivide( this ) );
        addSpecialFunction( new IntegerMod( this ) );

        addSpecialFunction( new ConcatenateStringsPrimitive( this ) );
        addSpecialFunction( new Sha1SumOf( this ) );
        addSpecialFunction( new UrlEncoding( this ) );
        addSpecialFunction( new UrlTarget( this ) );

        addSpecialFunction( new Combinator_B( this ) );
        addSpecialFunction( new Combinator_C( this ) );
        addSpecialFunction( new Combinator_I( this ) );
        addSpecialFunction( new Combinator_K( this ) );
        addSpecialFunction( new Combinator_L( this ) );
        addSpecialFunction( new Combinator_O( this ) );
        addSpecialFunction( new Combinator_R( this ) );
        addSpecialFunction( new Combinator_S( this ) );
        addSpecialFunction( new Combinator_T( this ) );
        addSpecialFunction( new Combinator_U( this ) );
        addSpecialFunction( new Combinator_V( this ) );
        addSpecialFunction( new Combinator_W( this ) );
        addSpecialFunction( new Combinator_Y( this ) );
        addSpecialFunction( new Combinator_w( this ) );
    }

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

        loadPrimitives();
    }

    public void importModel( final URL url, final String baseURI )
        throws WurfelException
    {
        s_logger.debug( "Importing model " + url.toString() +
            ( ( null == baseURI ) ? "" : " as " + baseURI ) );

        boolean verifyData = true;

        try
        {
            repository.addData( url, baseURI, RDFFormat.RDFXML, verifyData, s_adminListener );
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




    public void addStatement( Value subj, Value pred, Value obj )
        throws WurfelException
    {
        Resource subjResource = castToResource( subj );
        URI predUri = castToUri( pred );
        Statement st = model.getValueFactory().createStatement( subjResource, predUri, obj );

        model.getGraph().add( st );
    }


    private void extractRDF( OutputStream out )
        throws WurfelException
    {
        RdfXmlWriter writer = new RdfXmlWriter( out );

        try
        {
            // This pushes all statements added with addStatement to the repository.
            boolean joinBlankNodes = true;
//            repository.clear( s_adminListener );
            repository.addGraph( model.getGraph(), joinBlankNodes );
//            repository.addGraph( model.getGraph() );

            repository.extractRDF(
                writer,
                false,   // ontology -- not an ontology
                true,    // instances -- do extract non-schema statements (that's what I want, right?)
//                true,   // explicitOnly -- only save data we've created in this session
                false,   // explicitOnly -- extract all statements, not only explicitly added ones
                true );  // niceOutput -- do alphabetize by subject
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }

        catch ( AccessDeniedException e )
        {
            throw new WurfelException( e );
        }

//        updateModel();
    }

    public void saveAs( String fileName )
        throws WurfelException
    {
        OutputStream out;

        try
        {
            out = new FileOutputStream( fileName );
        }

        catch ( FileNotFoundException e )
        {
            throw new WurfelException( e );
        }

        extractRDF( out );

        try
        {
            out.close();
        }

        catch ( IOException e )
        {
            throw new WurfelException( e );
        }
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

    public void show( Resource subject )
        throws WurfelException
    {
        System.out.println( subject.toString() );

        Set<URI> predicates = model.getPredicates( subject );
        Iterator<URI> predIter = predicates.iterator();
        while ( predIter.hasNext() )
        {
            URI predicate = predIter.next();

            System.out.println( "    " + predicate.getLocalName() );

            Set<Value> objects = model.multiply( subject, predicate );
            Iterator<Value> objIter = objects.iterator();
            while ( objIter.hasNext() )
                System.out.println( "        " + objIter.next().toString() );
        }
    }








    private boolean isApply( Value v )
    {
        return v instanceof Apply;
    }

    private Value translateFromGraph( Value v )
    {
        if ( v instanceof URI )
        {
            Function f = specialFunctions.get( (URI) v );

            if ( null != f )
                return f;
            else
                return v;
        }

        else
            return v;
    }

    private Value translateToGraph( Value v )
    {
        if ( v instanceof Function )
            return ( (Function) v ).getUri();
        else
            return v;
    }

    // TODO: this should throw an exception if the identifier does not resolve
    //       to a unique value.
    public Value resolveIdentifier( String s )
        throws WurfelException
    {
        Value v = model.resolve( s );
        return ( v == null ) ? null : translateFromGraph( v );
    }

// FIXME: 'apply' is now a bit of a misnomer
    public Set<Value> apply( Value func, Value arg )
        throws WurfelException
    {
        arg = translateToGraph( arg );

// TODO: combinators and primitives as func will have their own, idiosyncratic ways of yielding a product
        Iterator<Value> resultIter = model.multiply( arg, func ).iterator();
        Set<Value> result = new NodeSet();
        while ( resultIter.hasNext() )
        {
            Value v = resultIter.next();
            result.add( translateFromGraph( v ) );
        }

        return result;
    }


    /**
     *  Carries out normal-order lazy beta reduction, distributing operations
     *  over node sets.
     */
    public NodeSet reduce( Value expr )
        throws WurfelException
    {
//System.out.println( "expr = " + expr.toString() );
        if ( isApply( expr ) && ( (Apply) expr ).arity() == 0 )
        {
            // Reduce the function.
            Iterator<Value> reducedFuncIter = reduce(
                ( (Apply) expr ).getFunction() ).iterator();

            // Reduce the function.
            Iterator<Value> reducedArgIter = reduce(
                ( (Apply) expr ).getArgument() ).iterator();

            // Iterate over the cartesian product of the reduced function(s)
            // with the reduced argument(s).
            LinkedList<Value> argList = new LinkedList<Value>();
            NodeSet result = new NodeSet();
            while ( reducedFuncIter.hasNext() )
            {
                Value function = reducedFuncIter.next();
                while ( reducedArgIter.hasNext() )
                {
                    Value argument = reducedArgIter.next();

                    // Apply the function to the argument.
                    Apply tmpApply = new Apply( function, argument );
                    if ( tmpApply.arity() == 0 )
                    {
                        argList.clear();

                        Collection<Value> itmResult = tmpApply.applyTo( argList, this );

                        // Reduction is recursive; we must first iterate over
                        // the intermediate results and reduce them before
                        // adding them to the list of final results.
                        Iterator<Value> itmIter = itmResult.iterator();
                        while ( itmIter.hasNext() )
                            result.add( reduce( itmIter.next() ) );
                    }

                    else
                        result.add( tmpApply );
                }
            }

            return result;
        }

        else
            return new NodeSet( expr );
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

    public void printStatements()
        throws WurfelException
    {
        updateModel();

        Set<Resource> subjects = model.getSubjects();
        Iterator<Resource> subjIter = subjects.iterator();
        while ( subjIter.hasNext() )
        {
            Resource subject = subjIter.next();
            show( subject );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
