package wurfel.model;

import wurfel.Context;
import wurfel.Wurfel;
import wurfel.WurfelException;

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.repository.Connection;

import java.util.Collection;
import java.util.List;
import java.util.ArrayList;

public class EvaluationContext
{
    private Context context;
    private Connection connection;

    private static URI
        s_xsdBooleanUri = null,
        s_xsdDoubleUri = null,
        s_xsdIntegerUri = null,
        s_xsdStringUri = null,
        s_rdfFirstUri = null,
        s_rdfRestUri = null,
        s_rdfNilUri = null;

    private static boolean s_initialized = false;

// TODO: these URIs should really be created via the model's ValueFactory.
    private static void initialize()
        throws WurfelException
    {
        s_xsdBooleanUri = Wurfel.getXmlSchemaUri( "boolean" );
        s_xsdDoubleUri = Wurfel.getXmlSchemaUri( "double" );
        s_xsdIntegerUri = Wurfel.getXmlSchemaUri( "integer" );
        s_xsdStringUri = Wurfel.getXmlSchemaUri( "string" );
        s_rdfFirstUri = Wurfel.getRdfUri( "first" );
        s_rdfRestUri = Wurfel.getRdfUri( "rest" );
        s_rdfNilUri = Wurfel.getRdfUri( "nil" );

        s_initialized = true;
    }

    ////////////////////////////////////////////////////////////////////////////

    public EvaluationContext( Context context )
        throws WurfelException
    {
        if ( !s_initialized )
            initialize();

        this.context = context;

        try
        {
            connection = context.getRepository().getConnection();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public Context getContext()
    {
        return context;
    }

    public Connection getConnection()
    {
        return connection;
    }

    public void close()
        throws WurfelException
    {
        try
        {
            connection.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public Collection<Value> findProduct( Value arg, Value func )
        throws WurfelException
    {
//        Apply a = new Apply( func, arg );
//        return reduce( a );
        return context.multiply( arg, func, this );
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

/*
    public Collection<Value> containerValue( final Resource head )
        throws WurfelException
    {


        try
        {
            Connection con = repository.getConnection();
            boolean includeInferred = false;
            CloseableIterator<? extends Statement> stmtIter
                = con.getStatements(
                    head, s_rdfTypeUri, s_swoogleQueryResponseUri, includeInferred );
            while ( stmtIter.hasNext() )
                results.add( stmtIter.next().getSubject() );
            stmtIter.close();
            conn.close();

            con.close();
        }

        catch ( OpenRDFException e )
        {
            throw new WurfelException( e );
        }
    }
*/

    ////////////////////////////////////////////////////////////////////////////

    public URI createUri( final String s )
    {
        return context.getRepository().getValueFactory().createURI( s );
    }

    public Literal createLiteral( final String s )
    {
        return context.getModel().getValueFactory().createLiteral( s, s_xsdStringUri );
    }

    public Literal createLiteral( final int i )
    {
        return context.getModel().getValueFactory().createLiteral( "" + i, s_xsdIntegerUri );
    }

    public Literal createLiteral( final double d )
    {
        return context.getModel().getValueFactory().createLiteral( "" + d, s_xsdDoubleUri );
    }

    ////////////////////////////////////////////////////////////////////////////

    public void addStatement( Value subj, Value pred, Value obj )
        throws WurfelException
    {
        Resource subjResource = castToResource( subj );
        URI predUri = castToUri( pred );

        try
        {
//            connection.add( subjResource, predUri, obj, singleContext );
            connection.add( subjResource, predUri, obj );
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
