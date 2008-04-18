/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.restpipe;

import net.fortytwo.rdfwiki.RDFRepresentation;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RDFSource;
import net.fortytwo.ripple.rdf.RDFUtils;
import org.openrdf.model.URI;
import org.openrdf.model.impl.URIImpl;
import org.openrdf.rio.RDFFormat;
import org.restlet.Context;
import org.restlet.data.MediaType;
import org.restlet.data.Request;
import org.restlet.data.Response;
import org.restlet.resource.Representation;
import org.restlet.resource.Variant;

import java.util.List;
import java.util.LinkedList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Author: josh
 * Date: Feb 6, 2008
 * Time: 2:13:18 PM
 */
public abstract class StatefulResource extends RPPResource {
    private static final Logger LOGGER = Logger.getLogger(StatefulResource.class.getName());

    protected URI selfUri;

    public StatefulResource(final Context context,
                            final Request request,
                            final Response response) throws Exception {
        super(context, request, response);

        selfUri = new URIImpl(request.getResourceRef().toString());
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
            return RDFUtils.getRdfVariants();
        } catch (RippleException e) {
            e.logError();
            return new LinkedList<Variant>();
        }
    }

    @Override
    public Representation getRepresentation(final Variant variant) {
        MediaType type = variant.getMediaType();

        try {
            RDFFormat format = RDFUtils.findRdfFormat(type);

            if (null == format) {
                return null;
            }

            return new RDFRepresentation(createRdfSource(), format);
        }

        catch (Throwable t) {
            LOGGER.log(Level.WARNING, "failed to create RDF representation", t);
            return null;
        }
    }

    protected abstract RDFSource createRdfSource() throws RippleException;
}