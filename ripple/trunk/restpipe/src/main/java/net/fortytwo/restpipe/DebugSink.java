package net.fortytwo.restpipe;

import org.restlet.resource.Variant;
import org.restlet.resource.Representation;

import java.util.List;
import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfUtils;

/**
 * Author: josh
 * Date: Feb 27, 2008
 * Time: 2:14:21 PM
 */
public class DebugSink implements RepresentationSink
{
    public String getComment()
    {
        return "just a debugging tool";    
    }

    public List<Variant> getVariants()
    {
        try
        {
            return RdfUtils.getRdfVariants();
        }

        catch (RippleException e)
        {
            e.logError();
            return new LinkedList<Variant>();
        }
    }

    public void put( final Representation rep ) throws RippleException
    {
        System.out.println( "received representation:\n" + rep );
        //throw new RippleException("just testing!");
    }
}
