package net.fortytwo.rdfwiki;

import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;
import org.openrdf.sail.SailConnection;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Resource;
import org.restlet.resource.StringRepresentation;
import org.restlet.resource.Variant;

public class NonInformationResource extends Resource
{
	private URI uri;

    public NonInformationResource(Context context, Request request,
            Response response)
    {
        super(context, request, response);
//        resourceId = (String) request.getAttributes().get("resourceId");
        String resourceId = request.getResourceRef().getRemainingPart();
        uri = RdfWiki.getSail().getValueFactory().createURI(
        		RdfWiki.getBaseUri() + resourceId );

        // Here we add the representation variants exposed
        getVariants().add( new Variant( MediaType.TEXT_PLAIN ) );
    }

    @Override
    public Representation getRepresentation(Variant variant)
    {
        Representation result = null;
        if (variant.getMediaType().equals(MediaType.TEXT_PLAIN)) {
            result = new StringRepresentation("Resource with URI \""
                    + uri + "\"");
        }
        return result;
    }
    
    private Representation getRdfRepresentation( final RDFFormat format )
    {
    	/*
    	try
    	{
    		SailConnection sc = RdfWiki.getSail().getConnection();
    		sc.getStatements( null, null, null, false, uri );
    		sc.close();
    	}
    	
    	catch ( Throwable t)
    	{
    		
		}
		*/
return null;
    }
}
