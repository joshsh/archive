package net.fortytwo.restpipe;

import net.fortytwo.ripple.util.Sink;
import org.restlet.resource.Representation;
import org.restlet.resource.Variant;

import java.util.List;

/**
 * Author: josh
 * Date: Feb 17, 2008
 * Time: 12:31:27 PM
 */
public interface RepresentationSink extends Sink<Representation>
{
	List<Variant> getVariants();
}
