package net.fortytwo.restpipe;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.VirtualHost;
import org.restlet.data.Protocol;

import java.util.HashMap;

public class RippleServer
{
    private static HashMap<String, RepresentationSink<RippleException>> uriToSinkMap
		    = new HashMap<String, RepresentationSink<RippleException>>();

    public static RepresentationSink<RippleException> getSink( final String uri )
    {
        return uriToSinkMap.get( uri );
    }

    public static void putSink( final String uri, final RepresentationSink sink )
    {
        uriToSinkMap.put( uri, sink );
    }

    public static void main( final String[] args )
	{
        try {
            Ripple.initialize();

            // Create a new Restlet component and add a HTTP server connector to it
            Component component = new Component();
            component.getServers().add( Protocol.HTTP, 8182 );
            VirtualHost host = component.getDefaultHost();

            RepresentationSink debugSink = new DebugSink();
            putSink( "debugSink", debugSink );

            //Router r = null;
            //host.attach( "/seed", r );

            // For now, registered sinks take up only part of the URI space.
            host.attach( "/registered/", Receiver.class );

            // Some other temporary stuff, for experimental purposes
            Restlet infoRestlet = new InfoRestlet();
            host.attach( "/info/", infoRestlet);
            //Restlet pipeRestlet = new PipeRestlet();
            //host.attach( "/pipe/", pipeRestlet );

            // Now, let's start the component!
            // Note that the HTTP server connector is also automatically started.
            component.start();
        } catch (Throwable t) {
            t.printStackTrace();
            System.exit(1);
        }
    }
}
