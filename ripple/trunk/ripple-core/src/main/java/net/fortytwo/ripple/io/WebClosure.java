package net.fortytwo.ripple.io;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.BNodeToUriFilter;
import net.fortytwo.ripple.rdf.RdfBuffer;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SesameInputAdapter;
import net.fortytwo.ripple.rdf.SingleContextPipe;
import net.fortytwo.ripple.util.StringUtils;
import net.fortytwo.ripple.util.UriMap;
import org.apache.log4j.Logger;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.rio.RDFHandler;
import org.restlet.data.MediaType;
import org.restlet.resource.Representation;

import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;

/**
 * Note: this tool stores metadata about web activity; if a suitable
 * dereferencer cannot be found for a URI, no metadata will be stored.
 * 
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 12:25:29 PM
 */
public class WebClosure  // TODO: the name is a little misleading...
{
	private static final Logger LOGGER = Logger.getLogger( WebClosure.class );

	private Map<String, ContextMemo> memos = new HashMap<String, ContextMemo>();

	// Maps media types to Rdfizers
	private Map<MediaType, Rdfizer> rdfizers = new HashMap<MediaType, Rdfizer>();

	// Maps URI schemes to Dereferencers
	private Map<String, UriDereferencer> dereferencers = new HashMap<String, UriDereferencer>();

    private UriMap uriMap;
	private ValueFactory valueFactory;

	public WebClosure( final UriMap uriMap, final ValueFactory vf )
	{
        this.uriMap = uriMap;
		valueFactory = vf;
	}

	public void addRdfizer( final MediaType mediaType, final Rdfizer rdfizer )
	{
		rdfizers.put( mediaType, rdfizer );
	}

	public void addDereferencer( final String scheme, final UriDereferencer uriDereferencer )
	{
		dereferencers.put( scheme, uriDereferencer );
	}

	public void addMemo( final String uri, final ContextMemo memo )
	{
		memos.put( uri, memo );
	}

	public Map<String, ContextMemo> getMemos()
	{
		return memos;
	}

	public ContextMemo.Status extend( final URI uri, final RdfSink resultSink ) throws RippleException
	{
		ContextMemo.Status status = extendPrivate( uri, resultSink );
		if ( ContextMemo.Status.Success != status )
		{
			// Note: exception information is not necessarily recorded
			LOGGER.info( "Failed to dereference URI <"
					+ StringUtils.escapeUriString( uri.toString() ) + ">: " + status );
		}

		return status;
	}

	private ContextMemo.Status extendPrivate( final URI uri, final RdfSink resultSink ) throws RippleException
	{
		// TODO: memos should be inferred in a scheme-specific way
		String memoUri = RdfUtils.inferContext( uri );

		ContextMemo memo;
		UriDereferencer dref;

		// Note: this URL should be treated as a "black box" once created; it
		// need not resemble the URI it was created from.
		String mapped;

		// Rules out an otherwise possible race condition
		synchronized ( memos )
		{
			memo = memos.get( memoUri );

			if ( null != memo )
			{
				return memo.getStatus();
			}

			try
			{
				mapped = uriMap.get( memoUri );
			}

			catch ( RippleException e )
			{
				return ContextMemo.Status.InvalidUri;
			}
	
			LOGGER.info( "Dereferencing URI <"
					+ StringUtils.escapeUriString( uri.toString() )
					+ "> at location " + mapped );

			try
			{
				dref = chooseDereferencer( mapped );
			}

			catch ( RippleException e )
			{
				e.logError( false );
				return ContextMemo.Status.InvalidUri;
			}

			if ( null == dref )
			{
				return ContextMemo.Status.BadUriScheme;
			}

			memo = new ContextMemo( ContextMemo.Status.Success );
			memos.put( memoUri, memo );
		}

		// Note: from this point on, failures are explicitly stored as caching
		// metadata.

		Representation rep;

		try
		{
			rep = dref.dereference( mapped );
		}

		catch ( RippleException e )
		{
			memo.setStatus( ContextMemo.Status.DereferencerError );
			return memo.getStatus();
		}

		MediaType mt = rep.getMediaType();
//System.out.println( "media type = " + mt );

		Rdfizer rfiz = chooseRdfizer( mt );
		if ( null == rfiz )
		{
			memo.setStatus( ContextMemo.Status.BadMediaType );
			memo.setMediaType( mt );
			return memo.getStatus();
		}

		URI context;

		try
		{
			context = valueFactory.createURI( memoUri );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Note: any pre-existing context information is discarded.
		RdfSink scp = new SingleContextPipe( resultSink, context, valueFactory );
		
		RdfBuffer results = new RdfBuffer( scp );
		RDFHandler hdlr = new SesameInputAdapter( Ripple.useBlankNodes()
				? results
				: new BNodeToUriFilter( results, valueFactory ) );

		InputStream is;

		try
		{
			is = rep.getStream();
		}

		catch ( IOException e )
		{
			throw new RippleException( e );
		}

		// For now...
		String baseUri = memoUri;

		ContextMemo.Status status;

		status = rfiz.handle( is, hdlr, uri, baseUri );

		if ( ContextMemo.Status.Success == status )
		{
			// Push results and record success
			results.flush();
		}

		memo.setStatus( status );
		memo.setMediaType( mt );

		return status;
	}

	private UriDereferencer chooseDereferencer( final String uri ) throws RippleException
	{
		String scheme;

		try
		{
			scheme = new java.net.URI( uri ).getScheme();
		}

		catch ( URISyntaxException e )
		{
			throw new RippleException( e );
		}

		return dereferencers.get( scheme );
	}

	private Rdfizer chooseRdfizer( final MediaType mediaType ) throws RippleException
	{
		return rdfizers.get( mediaType );
	}
}
