/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


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
import java.util.Set;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.Dereferencer;
import net.fortytwo.ripple.io.HttpUriDereferencer;
import net.fortytwo.ripple.io.RdfSourceAdapter;
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

public class Model
{
	final static Logger logger = Logger.getLogger( Model.class );

	String name;

	Repository repository;
public Repository getRepository()
{
	return repository;
}

	Dereferencer dereferencer;
public Dereferencer getDereferencer()
{
	return dereferencer;
}

	ModelBridge bridge;
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
		logger.debug( "Creating new Model '" + name + "'" );

		this.repository = repository;
		this.name = name;

		bridge = new ModelBridge();

		UrlFactory urlFactory = new UrlFactory();

		dereferencer = new HttpUriDereferencer( urlFactory );

		// Don't bother trying to dereference terms in these common namespaces.
		dereferencer.addFailureMemo( "http://www.w3.org/XML/1998/namespace#" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema" );
		dereferencer.addFailureMemo( "http://www.w3.org/2001/XMLSchema#" );

		// Don't try to dereference the cache resource.
		dereferencer.addSuccessMemo( "urn:net.fortytwo.ripple.store.meta" );

		loadSymbols( urlFactory );

		logger.debug( "Finished creating Model '" + name + "'" );
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
		final boolean override = Ripple.preferNewestNamespaceDefinitions();

		final ModelConnection mc = ( null == name )
			? new ModelConnection( this )
			: new ModelConnection( this, name );

		return mc;
	}

	////////////////////////////////////////////////////////////////////////////

	public void readAll( final RdfSink sink )
		throws RippleException
	{
		RdfSourceAdapter handler = new RdfSourceAdapter( sink );

		try
		{
			RepositoryConnection rc = repository.getConnection();
			rc.export( handler );
			rc.close();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	// Note: this may be a very expensive operation (see Sesame API).
	public long countStatements()
		throws RippleException
	{
		long size;

		try
		{
			RepositoryConnection rc = repository.getConnection();
			size = rc.size();
			rc.close();
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
		Collection<RippleValue> contexts = new LinkedList<RippleValue>();

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
}

// kate: tab-width 4
