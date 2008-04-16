/*
 * $URL: $
 * $Revision: $
 * $Author: $
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.restpipe;

import org.restlet.Context;
import org.restlet.resource.Variant;
import org.restlet.resource.Representation;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.data.Reference;
import net.fortytwo.ripple.RippleException;

import java.util.List;
import java.util.LinkedList;
import java.net.URI;
import java.net.URISyntaxException;

/**
 * Author: josh
 * Date: Apr 15, 2008
 * Time: 12:26:53 PM
 */
public class Pipe extends Receiver
{
    private static final String SINK = "sink";

    private Sender sender;

    public Pipe( final Context context,
                 final Request request,
                 final Response response ) throws RippleException
    {
        super( context, request, response );

        String sinkUri = getParameter( SINK );
        sender = new Sender( createURI( sinkUri ) );

        RepresentationSink<RippleException> repSink = new RepresentationSink<RippleException>()
        {
            public List<Variant> getVariants()
            {
                // TODO: this should be the same as the remote sink's list of variants
                return new LinkedList<Variant>();
            }

            public String getComment()
            {
                return "TODO";
            }

            public void put(Representation representation) throws RippleException
            {
                //To change body of implemented methods use File | Settings | File Templates.
            }
        };
    }
}
