/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.restpipe;

import java.util.List;
import java.util.LinkedList;
import java.net.URISyntaxException;

import org.restlet.Context;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.data.MediaType;
import org.restlet.data.Status;
import org.restlet.resource.Variant;
import org.restlet.resource.Representation;
import org.restlet.resource.StringRepresentation;
import org.openrdf.model.Statement;
import org.openrdf.model.Namespace;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.RDFS;
import org.openrdf.model.impl.NamespaceImpl;
import org.openrdf.rio.RDFFormat;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RDFCollector;
import net.fortytwo.ripple.rdf.RDFUtils;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.rdfwiki.RDFRepresentation;

public class Receiver extends RPPResource
{
    protected RepresentationSink<RippleException> sink;
    protected RDFCollector selfRepresentation = null;

    public Receiver( final Context context,
			final Request request,
            final Response response ) throws RippleException
	{            
        super( context, request, response );

        this.sink = RippleServer.getSink( request.getResourceRef().getLastSegment() );
    }

    @Override
    public boolean allowDelete()
    {
    	return false;
    }

    @Override
    public boolean allowGet()
    {
    	return true;
    }

    @Override
    public boolean allowPost()
    {
    	return true;
    }

    @Override
    public boolean allowPut()
    {
    	return true;
    }

    @Override
    public List<Variant> getVariants()
    {
    	return ( null == sink ) ? new LinkedList<Variant>() : sink.getVariants();
    }

    @Override
    public Representation getRepresentation( final Variant variant )
    {
        if ( null == sink )
        {
            return null;
        }

        try {
            return selfRepresentation( variant );
        } catch ( RippleException e ) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
            return null;
        }
    }

    @Override
    public void	post( final Representation entity )
    {
        if ( null == sink )
        {
            this.getResponse().setStatus( Status.CLIENT_ERROR_NOT_FOUND );
        }

        else if ( !isAllowedMediaType( entity.getMediaType() ) )
        {
            StringBuilder sb = new StringBuilder();
            sb.append( "Unsupported media type: " )
                    .append( entity.getMediaType() )
                    .append( ".  Should be one of: " );
            
            boolean first = true;
            for ( Variant v : getVariants() )
            {
                if ( first )
                {
                    first = false;
                }

                else
                {
                    sb.append( ", " );
                }

                sb.append( v.getMediaType() );
            }

            this.getResponse().setStatus(
                    Status.CLIENT_ERROR_UNSUPPORTED_MEDIA_TYPE, sb.toString() );
        }
        
        else
        {
            try {
                sink.put( entity );

                this.getResponse().setStatus( Status.SUCCESS_ACCEPTED );               
                // For some reason, we must explicitly set the response entity,
                // or the connection hangs.
                this.getResponse().setEntity( new StringRepresentation(
                        "input entity has been accepted and will be acted upon" ) );
            } catch (RippleException e) {
                // Display a basic error message, without a stack trace.
                // Ideally, this should include a logging ID as well.
                this.getResponse().setStatus( Status.SERVER_ERROR_INTERNAL, "Error: " + e.getMessage() );
            }
        }
    }

    @Override
    public void put( final Representation entity )
    {
        // For now, PUT is handled identially to POST
        post( entity );
    }

    ////////////////////////////////////////////////////////////////////////////

    private boolean isAllowedMediaType( final MediaType mt )
    {       
        for ( Variant v : getVariants() )
        {
            if ( v.getMediaType().equals( mt ) )
            {
                return true;
            }
        }

        return false;
    }

    private Representation selfRepresentation( final Variant variant ) throws RippleException
    {
        if ( null == selfRepresentation )
        {
            selfRepresentation = new RDFCollector();

            Sink<Namespace, RippleException> nsSink = selfRepresentation.namespaceSink();
            Sink<Statement, RippleException> stSink = selfRepresentation.statementSink();

            nsSink.put( new NamespaceImpl( "rdf", RDF.NAMESPACE ) );
            nsSink.put( new NamespaceImpl( "rdfs", RDFS.NAMESPACE ) );
            nsSink.put( new NamespaceImpl( "rpp", RPP.NAMESPACE ) );

            stSink.put( valueFactory.createStatement(
                    selfUri, RDF.TYPE, RPP.SINK ) );
            stSink.put( valueFactory.createStatement(
                    selfUri, RDFS.COMMENT, valueFactory.createLiteral( sink.getComment() ) ) );
            for ( Variant v : getVariants() )
            {
                stSink.put( valueFactory.createStatement( selfUri, RPP.INPUTTYPE,
                        valueFactory.createLiteral( v.getMediaType().getName() ) ) );
            }
        }

        MediaType type = variant.getMediaType();
//        MediaType type = ( null == variant ) ? new MediaType( "application/rdf+xml" ) : variant.getMediaType();

        RDFFormat format = RDFUtils.findRdfFormat( type );

        return ( null == format ) ? null : new RDFRepresentation( selfRepresentation, format );
    }

    protected static java.net.URI createURI( final String s ) throws RippleException
    {
        try {
            return new java.net.URI( s );
        } catch (URISyntaxException e) {
            throw new RippleException( e );
        }
    }
}
