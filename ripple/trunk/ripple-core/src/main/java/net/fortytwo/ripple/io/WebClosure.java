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
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 12:25:29 PM
 */
public class WebClosure  // TODO: the name is a little misleading...
{
	private static final Logger LOGGER = Logger.getLogger( WebClosure.class );

	// Maps media types to Rdfizers
	private Map<MediaType, Rdfizer> rdfizers = new HashMap<MediaType, Rdfizer>();

	// Maps URI schemes to Dereferencers
	private Map<String, UriDereferencer> dereferencers = new HashMap<String, UriDereferencer>();

	private Map<String, ContextMemo> memos = new HashMap<String, ContextMemo>();

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

	public Rdfizer.Outcome extend( final URI uri, final RdfSink resultSink ) throws RippleException
	{
		Rdfizer.Outcome outcome = extendPrivate( uri, resultSink );
		if ( Rdfizer.Outcome.Success != outcome )
		{
			failed( uri, outcome );
		}

		return outcome;
	}

	private Rdfizer.Outcome extendPrivate( final URI uri, final RdfSink resultSink ) throws RippleException
	{
		// TODO: memos should be inferred in a scheme-specific way
		String memoUri = RdfUtils.inferContext( uri );

		ContextMemo memo = memos.get( memoUri );
		if ( null != memo )
		{
			return memo.getOutcome();
		}

		// Note: this URL should be treated as a "black box" once created; it
		// need not resemble the URI it was created from.
		String mapped;

		try
		{
			mapped = uriMap.get( memoUri );
		}

		catch ( RippleException e )
		{
			// Don't bother remembering the URI.
			return Rdfizer.Outcome.InvalidUri;
		}
	
		LOGGER.info( "Dereferencing URI <"
				+ StringUtils.escapeUriString( uri.toString() )
				+ "> at location " + mapped );

		UriDereferencer dref;

		try
		{
			dref = chooseDereferencer( mapped );
		}

		catch ( RippleException e )
		{
			// Don't bother remembering the URI.
			return Rdfizer.Outcome.InvalidUri;
		}

		if ( null == dref )
		{
			// Don't bother remembering the URI.
			return Rdfizer.Outcome.NoDereferencer;
		}
		
		Representation rep;

		try
		{
			rep = dref.dereference( mapped );
		}

		catch ( RippleException e )
		{
			e.logError();
			addMemo( memoUri, new ContextMemo( Rdfizer.Outcome.DereferencerError ) );
			return Rdfizer.Outcome.DereferencerError;
		}

		// TODO: don't tolerate null representations from dereferencers; this is an error condition
		if ( null == rep )
		{
			addMemo( memoUri, new ContextMemo( Rdfizer.Outcome.DereferencerError ) );
			return Rdfizer.Outcome.DereferencerError;
		}

		MediaType mt = rep.getMediaType();
//System.out.println( "media type = " + mt );

		Rdfizer rfiz = chooseRdfizer( mt );
		if ( null == rfiz )
		{
			addMemo( memoUri, new ContextMemo( Rdfizer.Outcome.NoRdfizer ) );
			return Rdfizer.Outcome.NoRdfizer;
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

		Rdfizer.Outcome outcome;

		outcome = rfiz.handle( is, hdlr, uri, baseUri );

		if ( Rdfizer.Outcome.Success == outcome )
		{
			// Push results and record success
			results.flush();
		}

		else
		{
			// Record the failure and discard any results
			failed( uri, outcome );
		}

		addMemo( memoUri, new ContextMemo( outcome ) );

		return outcome;
	}

	private void failed( final URI uri, final Rdfizer.Outcome outcome )
	{
		// Note: exception information is not necessarily recorded
		LOGGER.info( "Failed to dereference URI <"
				+ StringUtils.escapeUriString( uri.toString() ) + ">: " + outcome );
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