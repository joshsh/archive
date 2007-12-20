package net.fortytwo.restpipe;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

import org.restlet.Client;
import org.restlet.data.Protocol;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.StringRepresentation;

public class PostSink implements Sink<String>
{
	private static final Logger LOGGER = Logger.getLogger( PostSink.class );

	private static Client client = new Client( Protocol.HTTP );

	private String uri;

	public PostSink( final String uri )
	{
		this.uri = uri;
	}

	public void put( final String s ) throws RippleException
	{
LOGGER.info( "posting string to <" + uri + ">: " + s );
		Representation rep = new StringRepresentation( s );

		Response response;

		synchronized ( client )
		{
			response = client.put( uri, rep );
		}
	}
}

// kate: tab-width 4
