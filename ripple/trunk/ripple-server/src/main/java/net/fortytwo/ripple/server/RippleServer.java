package net.fortytwo.ripple.server;

import net.fortytwo.ripple.Ripple;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.Server;
import org.restlet.data.ClientInfo;
import org.restlet.data.MediaType;
import org.restlet.data.Protocol;
import org.restlet.data.Request;
import org.restlet.data.Response;

public class RippleServer
{
	public static void main( final String[] args ) throws Exception
	{
		Ripple.initialize();

		// Create a new Restlet component and add a HTTP server connector to it
		Component component = new Component();
		component.getServers().add( Protocol.HTTP, 8182 );
		
		
		// Then attach it to the local host
		Restlet infoRestlet = new InfoRestlet();
// Note: the trailing slash prevents us from matching /informatics, /info_foo etc.
		component.getDefaultHost().attach( "/info/", infoRestlet);
		
		Restlet sinkRestlet = new SinkRestlet();
		component.getDefaultHost().attach( "/sink", sinkRestlet );

		Restlet pipeRestlet = new PipeRestlet();
		component.getDefaultHost().attach( "/pipe", pipeRestlet );

		// Now, let's start the component!
		// Note that the HTTP server connector is also automatically started.
		component.start();
	}

/*
	public static void main( final String[] args ) throws Exception
	{
		// Creating a minimal Restlet returning "Hello World"
		Restlet restlet = new Restlet() {
			@Override
			public void handle(Request request, Response response) {
				response.setEntity("Hello World!", MediaType.TEXT_PLAIN);
			}
		};
		
		// Create the HTTP server and listen on port 8182
		new Server(Protocol.HTTP, 8182, restlet).start();
	}
*/
}

// kate: tab-width 4