package net.fortytwo.restpipe;

import net.fortytwo.rdfwiki.RdfRepresentation;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfSource;
import net.fortytwo.ripple.rdf.RdfUtils;
import org.openrdf.model.URI;
import org.openrdf.model.impl.URIImpl;
import org.openrdf.rio.RDFFormat;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Resource;
import org.restlet.resource.Variant;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Author: josh
 * Date: Feb 6, 2008
 * Time: 2:13:18 PM
 */
public abstract class StatefulResource extends Resource {
    private static final Logger LOGGER = Logger.getLogger(StatefulResource.class.getName());

    protected URI selfUri;
    protected Map<String, String> params;

    public StatefulResource(final Context context,
                            final Request request,
                            final Response response) throws Exception {
        super(context, request, response);

        selfUri = new URIImpl(request.getResourceRef().toString());
        params = getParameters();
    }

    public boolean allowDelete() {
        return false;
    }

    public boolean allowGet() {
        return true;
    }

    public boolean allowPost() {
        return false;
    }

    public boolean allowPut() {
        return false;
    }

    @Override
    public List<Variant> getVariants() {
        try {
            return RdfUtils.getRdfVariants();
        } catch (RippleException e) {
            e.logError();
            return new LinkedList<Variant>();
        }
    }

    @Override
    public Representation getRepresentation(final Variant variant) {
        MediaType type = variant.getMediaType();

        try {
            RDFFormat format = RdfUtils.findRdfFormat(type);

            if (null == format) {
                return null;
            }

            return new RdfRepresentation(createRdfSource(), format);
        }

        catch (Throwable t) {
            LOGGER.log(Level.WARNING, "failed to create RDF representation", t);
            return null;
        }
    }

    protected abstract RdfSource createRdfSource() throws RippleException;

    // FIXME: this code is also in Receiver.java
    private Map<String, String> parameters;
    protected Map<String, String> getParameters()
    {
        if ( null == parameters )
        {
            parameters = new HashMap<String, String>();
            String uri = selfUri.toString();

            if ( uri.contains( "?" ) )
            {
                String query = uri.substring( uri.indexOf( "?" ) + 1 );
                String[] pairs = query.split( "&" );
                for ( int i = 0; i < pairs.length; i++ )
                {
                    String[] pair = pairs[i].split( "=" );
                    String key = pair[0];
                    String value = pair[1];
                    parameters.put( key, value );
                }
            }
        }

        return parameters;
    }
}