package net.fortytwo.restpipe;

import net.fortytwo.ripple.Ripple;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.VirtualHost;
import org.restlet.data.Protocol;

import java.util.HashMap;

public class RippleServer
{
    private static HashMap<String, RepresentationSink> uriToSinkMap
		    = new HashMap<String, RepresentationSink>();

    public static RepresentationSink getSink( final String uri )
    {
        return uriToSinkMap.get( uri );
    }

    public static void main( final String[] args ) throws Exception
	{
		Ripple.initialize();

		// Create a new Restlet component and add a HTTP server connector to it
		Component component = new Component();
		component.getServers().add( Protocol.HTTP, 8182 );
		VirtualHost host = component.getDefaultHost();

        RepresentationSink debugSink = new DebugSink();
        uriToSinkMap.put( "debugSink", debugSink );

        // For now, registered sinks take up only part of the URI space.
        host.attach( "/registered/", Receiver.class );

        // Some other temporary stuff, for experimental purposes
		Restlet infoRestlet = new InfoRestlet();
		host.attach( "/info/", infoRestlet);
		Restlet pipeRestlet = new PipeRestlet();
		host.attach( "/pipe/", pipeRestlet );
               
        // Now, let's start the component!
		// Note that the HTTP server connector is also automatically started.
		component.start();
	}
}
