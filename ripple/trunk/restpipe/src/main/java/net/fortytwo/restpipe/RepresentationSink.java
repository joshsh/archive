package net.fortytwo.restpipe;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.RippleException;
import org.restlet.resource.Representation;
import org.restlet.resource.Variant;

import java.util.List;

/**
 * Author: josh
 * Date: Feb 17, 2008
 * Time: 12:31:27 PM
 */
public interface RepresentationSink extends Sink<Representation, RippleException>
{
	List<Variant> getVariants();

    String getComment();
}
