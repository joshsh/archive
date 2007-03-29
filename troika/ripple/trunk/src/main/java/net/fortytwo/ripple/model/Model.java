package net.fortytwo.ripple.model;

import java.io.OutputStream;

import java.net.URL;

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

import net.fortytwo.ripple.UrlFactory;
import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

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
import org.openrdf.rio.trix.TriXWriter;
import org.openrdf.rio.RDFWriter;
import org.openrdf.model.Namespace;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.model.vocabulary.RDF;

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
			rplNs           = "http://fortytwo.net/2007/03/ripple/schema#",
			rplIntMathNs    = "http://fortytwo.net/2007/03/ripple/intmath#",
			rplJoyNs        = "http://fortytwo.net/2007/03/ripple/joy#",
			rplMiscNs       = "http://fortytwo.net/2007/03/ripple/misc#",
			rplStreamNs       = "http://fortytwo.net/2007/03/ripple/stream#",
			rplGraphNs        = "http://fortytwo.net/2007/03/ripple/graph#";

		Hashtable<String, String> urlMap = new Hashtable<String, String>();
		urlMap.put( rplNs,
			net.fortytwo.ripple.Ripple.class.getResource( "ripple.ttl" ) + "#" );
		urlMap.put( rplIntMathNs,
			net.fortytwo.ripple.extensions.intmath.IntMathExtension.class.getResource(
				"intmath.ttl" ) + "#" );
		urlMap.put( rplJoyNs,
			net.fortytwo.ripple.extensions.joy.JoyExtension.class.getResource(
				"joy.ttl" ) + "#" );
		urlMap.put( rplMiscNs,
			net.fortytwo.ripple.extensions.misc.MiscExtension.class.getResource(
				"ripple-misc.ttl" ) + "#" );
		urlMap.put( rplGraphNs,
			net.fortytwo.ripple.extensions.graph.GraphExtension.class.getResource(
				"graph.ttl" ) + "#" );
		urlMap.put( rplStreamNs,
			net.fortytwo.ripple.extensions.stream.StreamExtension.class.getResource(
				"stream.ttl" ) + "#" );

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
		bridge.add( Operator.OP, mc );

		try
		{
			( new net.fortytwo.ripple.extensions.misc.MiscExtension() ).load( mc );
			( new net.fortytwo.ripple.extensions.graph.GraphExtension() ).load( mc );
			( new net.fortytwo.ripple.extensions.intmath.IntMathExtension() ).load( mc );
			( new net.fortytwo.ripple.extensions.stream.StreamExtension() ).load( mc );
			( new net.fortytwo.ripple.extensions.joy.JoyExtension() ).load( mc );
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


	private static String
		// A special "URI" for metadata about Ripple's triple store.  It is not
		// dereferenceable, and its value is specific to the particular store.
		rplStoreRootUri = "urn:net.fortytwo.ripple.store.meta",
		rplStoreSuccessMemosUri = "http://fortytwo.net/2007/03/ripple/store#successMemos",
		rplStoreFailureMemosUri = "http://fortytwo.net/2007/03/ripple/store#failureMemos";

	public void load( URL url )
		throws RippleException
	{
System.out.println( "loading from URL: " + url );
		ModelConnection mc = new ModelConnection( this, "for Model load()" );

		mc.addGraph( url );

		// Restore dereferencer state by reading success and failure memos
		// from the last session (if present).
		{
			RdfValue meta = new RdfValue( mc.createUri( rplStoreRootUri ) );
			RdfValue succ = mc.findAtMostOneObject( meta,
				new RdfValue( mc.createUri( rplStoreSuccessMemosUri ) ) );
			RdfValue fail = mc.findAtMostOneObject( meta,
				new RdfValue( mc.createUri( rplStoreFailureMemosUri ) ) );

			if ( null != succ )
			{
				s_logger.debug( "reading success memos" );
				List<RippleValue> successMemos = mc.listValue( succ );

				for ( Iterator<RippleValue> iter = successMemos.iterator(); iter.hasNext(); )
					dereferencer.addSuccessMemo( mc.stringValue( iter.next() ) );
			}

			if ( null != fail )
			{
				s_logger.debug( "reading failure memos" );
				List<RippleValue> failureMemos = mc.listValue( fail );

				for ( Iterator<RippleValue> iter = failureMemos.iterator(); iter.hasNext(); )
					dereferencer.addFailureMemo( mc.stringValue( iter.next() ) );
			}
		}

		mc.close();
	}

	public void writeTo( OutputStream out )
		throws RippleException
	{
		// Save dereferencer state.
		{
			ModelConnection mc = new ModelConnection( this );

			RdfValue meta = new RdfValue( mc.createUri( rplStoreRootUri ) );
			RdfValue succ = new RdfValue( mc.createUri( rplStoreSuccessMemosUri ) );
			RdfValue fail = new RdfValue( mc.createUri( rplStoreFailureMemosUri ) );

			mc.removeStatementsAbout( meta, null );

			RdfValue rdfFirst = new RdfValue( RDF.FIRST );
			RdfValue rdfRest = new RdfValue( RDF.REST );
			RdfValue rdfNil = new RdfValue( RDF.NIL );
			RdfValue rdfType = new RdfValue( RDF.TYPE );
			RdfValue rdfList = new RdfValue( RDF.LIST );
			RdfValue last;

			last = null;
			for ( Iterator<String> iter = dereferencer.getSuccessMemos().iterator(); iter.hasNext(); )
			{
				// Note: apparently it's important (in Sesame 2 beta) to add an
				//       edge TO a blank node before adding edges FROM the blank
				//       node, otherwise you get a disjointed graph.  I don't
				//       know whether this is a bug or a feature.
				RdfValue cur = new RdfValue( mc.createBNode() );
				if ( null == last )
				{
					mc.add( meta, succ, cur );
					mc.add( cur, rdfType, rdfList );
				}
				else
					mc.add( last, rdfRest, cur );

				RdfValue first = mc.createValue( iter.next() );
				mc.add( cur, rdfFirst, first );

				last = cur;
			}
			if ( null != last )
				mc.add( last, rdfRest, rdfNil );

			last = null;
			for ( Iterator<String> iter = dereferencer.getFailureMemos().iterator(); iter.hasNext(); )
			{
				RdfValue cur = new RdfValue( mc.createBNode() );
				if ( null == last )
				{
					mc.add( meta, fail, cur );
					mc.add( cur, rdfType, rdfList );
				}
				else
					mc.add( last, rdfRest, cur );

				RdfValue first = mc.createValue( iter.next() );
				mc.add( cur, rdfFirst, first );

				last = cur;
			}
			if ( null != last )
				mc.add( last, rdfRest, rdfNil );

			mc.close();
		}

		// Note: a comment by Jeen suggests that a new writer should be created
		//       for each use:
		//       http://www.openrdf.org/forum/mvnforum/viewthread?thread=785#3159
		RDFWriter writer = new RDFXMLPrettyWriter( out );

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

	public void writeTrix( OutputStream out )
		throws RippleException
	{
		// Note: a comment by Jeen suggests that a new writer should be created
		//       for each use:
		//       http://www.openrdf.org/forum/mvnforum/viewthread?thread=785#3159
		RDFWriter writer = new TriXWriter( out );

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

	public Collection<RippleValue> getContexts()
		throws RippleException
	{
		Container contexts = new Container();

		try
		{
			RepositoryConnection conn = repository.getConnection();

			RepositoryResult<Resource> contextIter
				= conn.getContextIDs();
			while ( contextIter.hasNext() )
				contexts.add( new RdfValue( contextIter.next() ) );
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

// kate: tab-width 4
