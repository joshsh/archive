/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.restpipe;

import org.openrdf.model.URI;
import org.openrdf.model.impl.URIImpl;

/**
 * REST Pipeline Protocol ontology
 * 
 * Author: josh
 * Date: Apr 16, 2008
 * Time: 10:35:00 AM
 */
public interface RPP
{
    public static final String NAMESPACE = "http://fortytwo.net/2008/02/rpp#";
    
    public static final URI
            INPUTTYPE = new URIImpl( NAMESPACE + "inputType" ),
            SINK = new URIImpl( NAMESPACE + "Sink" );
}
