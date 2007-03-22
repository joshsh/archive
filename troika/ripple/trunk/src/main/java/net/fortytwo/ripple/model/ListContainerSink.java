package net.fortytwo.ripple.model;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class ListContainerSink extends Container implements Sink<RippleList>
{
    public ListContainerSink()
    {
        super();
    }

    public void put( RippleList v )
        throws RippleException
    {
//System.out.println( this + ".put( " + v + " )" );
//System.out.flush();
        add( v );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
