package net.fortytwo.rdfwiki;

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import net.fortytwo.ripple.rdf.RdfCollector;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.SailInserter;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.rdf.SingleContextPipe;
import net.fortytwo.ripple.util.CloseableIterationSource;

import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.SailConnection;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Resource;
import org.restlet.resource.Variant;

public class NonInformationResource extends Resource
{
	private static final Logger LOGGER
		= Logger.getLogger( NonInformationResource.class.getName() );
	
	private URI uri;

    public NonInformationResource( Context context, Request request,
            Response response)
    {
        super(context, request, response);
        
        uri = RdfWiki.getSail().getValueFactory().createURI(
        		request.getResourceRef().toString() );
System.out.println( "uri = " + uri );

        // Here we add the representation variants exposed
//        getVariants().add( new Variant() );
//        getVariants().add( new Variant( MediaType.TEXT_PLAIN ) );
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
    	return RdfWiki.getRdfVariants();
    }
    
    @Override
    public Representation getRepresentation( final Variant variant )
    {
        MediaType type = variant.getMediaType();
        RDFFormat format = RdfWiki.findRdfFormat( type );
        
        if ( null != format )
        {
        	return getRdfRepresentation( format );
        }
        
        /*
        if (variant.getMediaType().equals(MediaType.TEXT_PLAIN)) {
            result = new StringRepresentation("Resource with URI \""
                    + uri + "\"");
        }*/
        
        return null;
    }
    
    public void	post( final Representation entity )
    {
    	SailConnection sc = null;
    	boolean open = false;
    	
    	try
    	{
    		// Note: the resource URI might not be very useful as a base URI.
    		RdfRepresentation rep = new RdfRepresentation( entity, uri.toString() );

    		sc = RdfWiki.getSail().getConnection();
    		open = true;
    		RdfSink contextPipe = new SingleContextPipe(
    				new SesameOutputAdapter( new SailInserter(sc ) ),
    				uri, RdfWiki.getSail().getValueFactory() );
    		rep.getSource().writeTo( contextPipe );
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
    
    public void put( final Representation entity )
    {
    	delete();
    	post( entity );
    }
    
    public void delete()
    {
    	boolean open = false;
    	SailConnection sc = null;
    	
    	try
    	{
    		sc = RdfWiki.getSail().getConnection();
    		open = true;
    		sc.removeStatements( null, null, null, uri );
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
    		sc = RdfWiki.getSail().getConnection();
    		open = true;
    		CloseableIterationSource source	= new CloseableIterationSource(
    			sc.getStatements( null, null, null, false, uri ) );
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
