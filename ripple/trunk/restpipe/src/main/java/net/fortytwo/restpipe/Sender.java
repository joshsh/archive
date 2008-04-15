package net.fortytwo.restpipe;

import java.net.URI;

import org.restlet.Client;
import org.restlet.data.Method;
import org.restlet.data.Protocol;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Resource;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;

public class Sender implements Sink<Resource, RippleException>
{
	private URI remoteUri;
	private Client client;
	
	public Sender( final URI remoteUri )
	{
		this.remoteUri = remoteUri;
		client = new Client( Protocol.HTTP );
	}

	public void put( final Resource r ) throws RippleException
	{
		Request request = new Request( Method.POST, remoteUri.toString() );
		
		// TODO: the appropriate Variant should be determined by dereferencing
		// the remote URI.
		request.setEntity( r.getRepresentation( r.getPreferredVariant() ) );
		
//		request.setReferrerRef("http://www.mysite.org");
		
		Response response = client.handle( request );

		// TODO: examine the response and do something intelligent with it
		
Representation output = response.getEntity();
try {
output.write(System.out);
} catch ( Exception e ) {
throw new RippleException( e );
}
	}
}
