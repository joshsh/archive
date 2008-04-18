/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.restpipe;

import net.fortytwo.ripple.flow.Mapping;
import org.restlet.resource.Representation;

/**
 * Author: josh
 * Date: Apr 15, 2008
 * Time: 12:17:18 PM
 */
public interface RepresentationMapping<E extends Exception> extends Mapping<Representation, Representation, E> {
}
