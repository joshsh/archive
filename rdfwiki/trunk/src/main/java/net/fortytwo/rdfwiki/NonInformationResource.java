package net.fortytwo.rdfwiki;

import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfCollector;
import net.fortytwo.ripple.util.CloseableIterationSource;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Statement;
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

    public NonInformationResource(Context context, Request request,
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
    
    private Representation getRdfRepresentation( final RDFFormat format )
    {
    	try
    	{
    		SailConnection sc = RdfWiki.getSail().getConnection();
    		CloseableIterationSource source	= new CloseableIterationSource(
    			sc.getStatements( null, null, null, false, uri ) );
    		final RdfCollector collector = new RdfCollector();
Sink<Statement> loudSink = new Sink<Statement>(){
	public void put( final Statement st ) throws RippleException
	{
		System.out.println( "    putting statement: " + st );
		collector.statementSink().put( st );
	}
};
//    		source.writeTo( collector.statementSink() );
source.writeTo(loudSink);
			sc.close();
    		return new RdfRepresentation( collector, format );
    	}
    	
    	catch ( Throwable t)
    	{
    		LOGGER.log( Level.WARNING, "failed to create RDF representation", t );
    		return null;
		}
    }
}
