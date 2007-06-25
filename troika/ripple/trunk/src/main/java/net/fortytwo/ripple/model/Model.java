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

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.ExtensionLoader;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.UrlFactory;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
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

	/**
	*  @param Repository  an initialized Repository
	*/
	public Model( final Repository repository, final String name )
		throws RippleException
	{
		s_logger.debug( "Creating new Model '" + name + "'" );

		this.repository = repository;
		this.name = name;

		bridge = new ModelBridge();

		UrlFactory urlFactory = new UrlFactory();

		dereferencer = new HttpUriDereferencer( urlFactory );

		// Don't bother trying to dereference terms in these common namespaces.
		dereferencer.addFailureMemo( "http://www.w3.org/XML/1998/namespace#" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema#" );

		loadSymbols( urlFactory );

		s_logger.debug( "Finished creating Model '" + name + "'" );
	}

	private void loadSymbols( final UrlFactory uf )
		throws RippleException
	{
		ModelConnection mc = getConnection( "for Model.loadSymbols" );

		// At the moment, op needs to be a special value for the sake of the
		// evaluator.  This has the side-effect of making it a keyword.
		bridge.add( Operator.OP, mc );

		ExtensionLoader loader = new ExtensionLoader();

		try
		{
			loader.load( uf, mc );
		}

		catch ( RippleException e )
		{
			mc.close();
			throw e;
		}

		mc.close();
	}

	public ModelConnection getConnection( final String name )
		throws RippleException
	{
		final ModelConnection mc = ( null == name )
			? new ModelConnection( this )
			: new ModelConnection( this, name );

		Sink<Statement> addStatementSink = new Sink<Statement>()
		{
			public void put( Statement st )
				throws RippleException
			{
				mc.add( st );
			}
		};

		Sink<Namespace> defineNamespaceSink = new Sink<Namespace>()
		{
			public void put( Namespace ns )
				throws RippleException
			{
				mc.setNamespace( ns.getPrefix(), ns.getName() );
			}
		};

		RdfSourceAdapter adapter = new RdfSourceAdapter(
			addStatementSink,
			defineNamespaceSink,
			new NullSink<String>() );

		mc.setSourceAdapter( adapter );

		return mc;
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
		s_logger.info( "loading Model from URL: " + url );

		ModelConnection mc = getConnection( "for Model load()" );

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
			ModelConnection mc = getConnection( null );

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
				s_logger.debug( "writing success memos" );

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
				s_logger.debug( "writing failure memos" );

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

	////////////////////////////////////////////////////////////////////////////

	public void touch()
	{
		setChanged();
		notifyObservers();
	}
}

// kate: tab-width 4
