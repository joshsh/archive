package net.fortytwo.ripple.extensions.newstuff;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Scrap extends Combinator
{
    public Scrap( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createUri( NewExtension.getBaseUri() + "scrap" ), mc );
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws WurfelException
    {
        // Do nothing.
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
