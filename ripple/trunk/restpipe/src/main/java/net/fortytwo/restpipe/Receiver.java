package net.fortytwo.restpipe;

import java.net.URI;
import java.net.URISyntaxException;

import org.restlet.Context;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Resource;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class Receiver extends Resource
{
	private Sink<Resource> sink;
	private URI selfUri;

	public Receiver( final Context context,
			final Request request,
            final Response response,
            final Sink<Resource> sink ) throws RippleException
	{
        super( context, request, response );

		this.sink = sink;
		
        try
        {
			selfUri = new URI(
					request.getResourceRef().toString() );
		}
        
        catch ( URISyntaxException e )
        {
			throw new RippleException( e );
		}
	}
	
	...
}
