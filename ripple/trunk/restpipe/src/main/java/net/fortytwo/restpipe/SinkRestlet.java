package net.fortytwo.restpipe;

import java.util.HashMap;

import net.fortytwo.ripple.util.Sink;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.Server;
import org.restlet.data.CharacterSet;
import org.restlet.data.ClientInfo;
import org.restlet.data.Encoding;
import org.restlet.data.Language;
import org.restlet.data.MediaType;
import org.restlet.data.Preference;
import org.restlet.data.Protocol;
import org.restlet.data.Reference;
import org.restlet.data.Request;
import org.restlet.data.Response;

public class SinkRestlet extends Restlet
{
	private HashMap<String, Sink<String>> uriToSinkMap
		= new HashMap<String, Sink<String>>();

	@Override
	public void handle( final Request request, final Response response )
	{
		Reference resourceRef = request.getResourceRef();
		String uri = resourceRef.toString();

StringBuilder message = new StringBuilder();
message.append( "Sink URI: " + uri );
response.setEntity( message.toString(), MediaType.TEXT_PLAIN );
	}
}

// kate: tab-width 4
