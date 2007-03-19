package net.fortytwo.ripple.model;

import net.fortytwo.ripple.UrlFactory;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

//import org.openrdf.OpenRDFException;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.query.QueryLanguage;
import org.openrdf.query.GraphQueryResult;
import org.openrdf.query.TupleQueryResult;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.Repository;
import org.openrdf.rio.rdfxml.util.RDFXMLPrettyWriter;
import org.openrdf.rio.rdfxml.RDFXMLWriter;
import org.openrdf.model.Namespace;
import org.openrdf.repository.RepositoryResult;

import java.io.IOException;
import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Set;

import org.apache.log4j.Logger;

public class Model extends Observable
{
    private Dereferencer dereferencer;
public Dereferencer getDereferencer()
{
    return dereferencer;
}

    private final static Logger s_logger = Logger.getLogger( Model.class );

    String name;
    Repository repository;

    private ModelBridge bridge;
    public ModelBridge getBridge()
    {
        return bridge;
    }

    private UrlFactory createUrlFactory()
        throws RippleException
    {
        String
            rplNs = "http://fortytwo.net/2007/03/rpl#",
            rplMiscNs = "http://fortytwo.net/2007/03/rpl-misc#",
            rplTestNs = "http://fortytwo.net/2007/03/rpl-test#",
            rplNewNs = "http://fortytwo.net/2007/03/rpl-new#";

        Hashtable<String, String> urlMap = new Hashtable<String, String>();
        urlMap.put( rplNs,
            net.fortytwo.ripple.Ripple.class.getResource( "ripple.ttl" ) + "#" );
        urlMap.put( rplTestNs,
            net.fortytwo.ripple.extensions.test.TestExtension.class.getResource(
                "rpl-test.ttl" ) + "#" );
        urlMap.put( rplMiscNs,
            net.fortytwo.ripple.extensions.misc.MiscExtension.class.getResource(
                "rpl-misc.ttl" ) + "#" );
        urlMap.put( rplNewNs,
            net.fortytwo.ripple.extensions.newstuff.NewExtension.class.getResource(
                "rpl-new.ttl" ) + "#" );

        return new UrlFactory( urlMap );
    }

    /**
     *  @param Repository  an initialized Repository
     */
    public Model( final Repository repository, final String name )
        throws RippleException
    {
        s_logger.debug( "Creating new Model '" + name + "'" );

        this.repository = repository;
        this.name = name;

        dereferencer = new HttpUriDereferencer( createUrlFactory() );

        bridge = new ModelBridge();

        ModelConnection mc = new ModelConnection( this, "for Model constructor" );

        try
        {
            ( new net.fortytwo.ripple.extensions.test.TestExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.misc.MiscExtension() ).load( mc );
            ( new net.fortytwo.ripple.extensions.newstuff.NewExtension() ).load( mc );
        }

        catch ( RippleException e )
        {
            mc.close();
            throw e;
        }

        mc.close();
    }

public Repository getRepository()
{
    return repository;
}

    public void writeTo( OutputStream out )
        throws RippleException
    {
        // Note: a comment by Jeen suggests that a new writer should be created
        //       for each use:
        //       http://www.openrdf.org/forum/mvnforum/viewthread?thread=785#3159
        RDFXMLWriter writer = new RDFXMLPrettyWriter( out );

        try
        {
            RepositoryConnection con = repository.getConnection();
            con.export( writer );
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public void saveAs( String fileName )
        throws RippleException
    {
        OutputStream out;

        try
        {
            out = new FileOutputStream( fileName );
        }

        catch ( FileNotFoundException e )
        {
            throw new RippleException( e );
        }

        writeTo( out );

        try
        {
            out.close();
        }

        catch ( IOException e )
        {
            throw new RippleException( e );
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    public void multiply( RdfValue subj, RdfValue pred, Sink<RdfValue> sink )
        throws RippleException
    {
//        Value rdfSubj = bridge.getRdfEquivalentOf( subj, mc ).getRdfValue();
//        Value rdfPred = bridge.getRdfEquivalentOf( pred, mc ).getRdfValue();

        Value rdfSubj = subj.toRdf();
        Value rdfPred = pred.toRdf();

//if ( null == rdfSubj || null == rdfPred )
//return;
        try
        {
            dereferencer.dereference( subj );
        }

        catch ( RippleException e )
        {
            // (soft fail)
s_logger.debug( "Failed to dereference URI: " + rdfSubj );
        }

        if ( rdfSubj instanceof Resource && rdfPred instanceof URI )
        {
            try
            {
                RepositoryResult<Statement> stmtIter
                    = conn.getStatements(
                        (Resource) subject, (URI) rdfPred, null, Ripple.useInference() );
                while ( stmtIter.rdfSubj() )
                    sink.put( new RdfValue( stmtIter.next().getObject() ) );
            }

            catch ( Throwable t )
            {
                throw new RippleException( t );
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    // Note: this may be a very expensive operation (see Sesame API).
    public long countStatements()
        throws RippleException
    {
        long size;

        try
        {
            RepositoryConnection con = repository.getConnection();
            size = con.size();
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return size;
    }

    public void showNamespaces()
        throws RippleException
    {
        try
        {
            RepositoryConnection conn = repository.getConnection();

            RepositoryResult<Namespace> nsIter
                 = conn.getNamespaces();
            int maxlen = 0;
            while ( nsIter.hasNext() )
            {
                Namespace ns = nsIter.next();
                int len = ns.getPrefix().length();
                if ( len > maxlen )
                    maxlen = len;
            }
            nsIter.close();

            nsIter = conn.getNamespaces();
            while ( nsIter.hasNext() )
            {
                Namespace ns = nsIter.next();
                String prefix = ns.getPrefix();
                int len = prefix.length();
                System.out.print( prefix + ":" );
                for ( int i = 0; i < maxlen - len + 2; i++ )
                    System.out.print( " " );
                System.out.println( ns.getName() );
            }
            nsIter.close();

            conn.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    public Collection<Value> getContexts()
        throws RippleException
    {
        Container contexts = new Container();

        try
        {
            RepositoryConnection conn = repository.getConnection();

            RepositoryResult<Resource> contextIter
                 = conn.getContextIDs();
            while ( contextIter.hasNext() )
                contexts.add( contextIter.next() );
            contextIter.close();

            conn.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return contexts;
    }

    public Collection<Statement> graphQuery( final String queryStr )
        throws RippleException
    {
        Collection<Statement> statements = new ArrayList<Statement>();

        try
        {
            RepositoryConnection con = repository.getConnection();
            GraphQueryResult result = con.prepareGraphQuery(
                QueryLanguage.SERQL, queryStr ).evaluate();
//                QueryLanguage.SERQL, "CONSTRUCT * FROM {x} p {y}");

// TODO: can I expect the Statements to remain valid after the connection is closed?
            while ( result.hasNext() )
                statements.add( result.next() );

            result.close();
            con.close();
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }

        return statements;
    }

    ////////////////////////////////////////////////////////////////////////////

    public void touch()
    {
        setChanged();
        notifyObservers();
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
