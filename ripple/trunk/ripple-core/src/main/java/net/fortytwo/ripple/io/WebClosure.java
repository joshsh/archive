package net.fortytwo.ripple.io;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.StringUtils;
import net.fortytwo.ripple.util.UrlFactory;
import net.fortytwo.ripple.rdf.*;
import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.rio.RDFHandler;
import org.restlet.data.MediaType;
import org.restlet.resource.Representation;
import org.apache.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.util.*;

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

	private Set<String> successMemos = new HashSet<String>();
	private Set<String> failureMemos = new HashSet<String>();

    private UrlFactory urlFactory;
	private ValueFactory valueFactory;

	public WebClosure( final UrlFactory urlFactory, final ValueFactory vf )
	{
        this.urlFactory = urlFactory;
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

	public void addSuccessMemo( final String memo )
	{
//System.out.println( "adding success memo: " + memo );
		successMemos.add( memo );
	}

	public void addFailureMemo( final String memo )
	{
//System.out.println( "adding failure memo: " + memo );
		failureMemos.add( memo );
	}

	public Collection<String> getSuccessMemos()
	{
		return successMemos;
	}

	public Collection<String> getFailureMemos()
	{
		return failureMemos;
	}

	public Rdfizer.Outcome extend( final URI uri, final RdfSink resultSink ) throws RippleException
	{
		// TODO: memos should be inferred in a scheme-specific way
		String memo = RdfUtils.inferContext( uri );

		if ( successMemos.contains( memo ) )
		{
			return Rdfizer.Outcome.Success;
		}

		if ( failureMemos.contains( memo ) )
		{
			return Rdfizer.Outcome.Failure;
		}

		// Note: this URL should be treated as a "black box" once created; it
		// need not resemble the URI it was created from.
		String mapped = urlFactory.createUrl( memo ).toString();

		LOGGER.info( "Dereferencing URI <"
				+ StringUtils.escapeUriString( uri.toString() )
				+ "> at location " + mapped );

		UriDereferencer dref = chooseDereferencer( mapped );
		if ( null == dref )
		{
			addFailureMemo( memo );
			failed( uri, "no suitable dereferencer found" );
			return Rdfizer.Outcome.Failure;
		}
		
		Representation rep = dref.handle( mapped );
		if ( null == rep )
		{
			addFailureMemo( memo );
			failed( uri, "no representation could be created" );
			return Rdfizer.Outcome.Failure;
		}

		MediaType mt = rep.getMediaType();
//System.out.println( "media type = " + mt );

		Rdfizer rfiz = chooseRdfizer( mt );
		if ( null == rfiz )
		{
			addFailureMemo( memo );
			failed( uri, "no suitable rdfizer found" );
			return Rdfizer.Outcome.Failure;
		}

		URI context;

		try
		{
			context = valueFactory.createURI( memo );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		// Note: any pre-existing context information is discarded.
		RdfSink scp = new SingleContextPipe( resultSink, context, valueFactory );
		
		RdfBuffer results = new RdfBuffer( scp );
		RDFHandler hdlr = new SesameInputAdapter( results );

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
		String baseUri = memo;

		Rdfizer.Outcome outcome = rfiz.handle( is, hdlr, uri, baseUri );

		switch ( outcome )
		{
			case Success:
				// Push results and record success
				results.flush();
				addSuccessMemo( memo );
				break;
			case Failure:
				// Record the failure and discard any results
				addFailureMemo( memo );
				failed( uri, "rdfization failed" );
				break;
		}

		return outcome;
	}

	private void failed( final URI uri, final String msg )
	{
		// Note: exception information is not necessarily recorded
		LOGGER.info( "Failed to dereference URI <"
				+ StringUtils.escapeUriString( uri.toString() ) + ">: " + msg );
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
