/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.rdfwiki;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.rdf.SailInserter;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.CloseableIterationSource;
import net.fortytwo.ripple.rdf.RdfCollector;
import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Resource;
import org.restlet.resource.Variant;

import java.util.List;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class InformationResource extends Resource
{
	private static final Logger LOGGER
		= Logger.getLogger( InformationResource.class.getName() );
	
	protected URI selfUri;
	protected Sail sail;

    public InformationResource( final Context context, final Request request,
            final Response response ) throws Exception
    {
        super( context, request, response );

        sail = RdfWiki.getWiki( context ).getSail( request );
        
        selfUri = sail.getValueFactory().createURI(
        		request.getResourceRef().toString() );
//System.out.println( "selfUri = " + selfUri );
    }

    public boolean allowDelete()
    {
    	return true;
    }
    
    public boolean allowGet()
    {
    	return true;
    }
    
    public boolean allowPost()
    {
    	return true;
    }
    
    public boolean allowPut()
    {
    	return true;
    }
    
    @Override
    public List<Variant> getVariants()
    {
        try
        {
            return RdfUtils.getRdfVariants();
        }

        catch (RippleException e)
        {
            e.logError();
            return new LinkedList<Variant>();
        }
    }
    
    @Override
    public Representation getRepresentation( final Variant variant )
    {
        MediaType type = variant.getMediaType();
        RDFFormat format = null;
        try
        {
            format = RdfUtils.findRdfFormat( type );
        }

        catch (RippleException e)
        {
            e.logError();

            // TODO: this will probably come across as a 404 error, which is not accurate
            return null;
        }

        if ( null != format )
        {
        	return getRdfRepresentation( format );
        }

        /*
        if (variant.getMediaType().equals(MediaType.TEXT_PLAIN)) {
            result = new StringRepresentation("Resource with URI \""
                    + selfUri + "\"");
        }*/
        
        return null;
    }

    @Override
    public void	post( final Representation entity )
    {
    	SailConnection sc = null;
    	boolean open = false;
    	
    	try
    	{
    		// Note: the resource URI might not be very useful as a base URI.
    		RdfRepresentation rep = new RdfRepresentation( entity, selfUri.toString() );

    		sc = sail.getConnection();
    		open = true;
//    		RdfSink contextPipe = new SingleContextPipe(
//    				new SesameOutputAdapter( new SailInserter(sc ) ),
//    				selfUri, sail.getValueFactory() );
//    		rep.getSource().writeTo( contextPipe );
    		
    		// Don't filter statement context for now.
    		rep.getSource().writeTo( new SesameOutputAdapter( new SailInserter( sc ) ) );
    		
    		sc.commit();
    		sc.close();
    		open = false;
    	}
    	
    	catch ( Throwable t )
    	{
    		if ( open )
    		{
    			try
    			{
    				sc.close();
    			}
    			
    			catch ( Throwable t2 )
    			{
    				LOGGER.log( Level.SEVERE, "failed to close connection", t2 );
    			}
    		}
    		
    		getLogger().log( Level.WARNING, "failed to post representation", t );
    	}
    }
    
    @Override
    public void put( final Representation entity )
    {
    	delete();
    	post( entity );
    }
    
    @Override
    public void delete()
    {
    	boolean open = false;
    	SailConnection sc = null;
    	
    	try
    	{
    		sc = sail.getConnection();
    		open = true;
    		
//    		sc.removeStatements( null, null, null, uri );
    		sc.removeStatements( selfUri, null, null );
    		
    		sc.commit();
			sc.close();
			open = false;
    	}
    	
    	catch ( Throwable t)
    	{
    		if ( open )
    		{
    			try
    			{
    				sc.close();
    			}
    			
    			catch ( Throwable t2 )
    			{
    				LOGGER.log( Level.SEVERE, "failed to close connection", t2 );
    			}
    		}
    		
    		LOGGER.log( Level.WARNING, "failed to create RDF representation", t );
		}
    }
    
    private Representation getRdfRepresentation( final RDFFormat format )
    {
    	boolean open = false;
    	SailConnection sc = null;
    	
    	try
    	{
    		sc = sail.getConnection();
    		open = true;
    		CloseableIterationSource source	= new CloseableIterationSource(
//    			sc.getStatements( null, null, null, false, uri ) );
    			sc.getStatements( selfUri, null, null, false ) );
 			final RdfCollector collector = new RdfCollector();
    		source.writeTo( collector.statementSink() );
			sc.close();
			open = false;
    		return new RdfRepresentation( collector, format );
    	}
    	
    	catch ( Throwable t)
    	{
    		if ( open )
    		{
    			try
    			{
    				sc.close();
    			}
    			
    			catch ( Throwable t2 )
    			{
    				LOGGER.log( Level.SEVERE, "failed to close connection", t2 );
    			}
    		}
    		
    		LOGGER.log( Level.WARNING, "failed to create RDF representation", t );
    		return null;
		}
    }
}
