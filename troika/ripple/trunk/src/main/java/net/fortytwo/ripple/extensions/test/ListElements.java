package net.fortytwo.ripple.extensions.test;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class ListElements extends PrimitiveFunction
{
    public ListElements( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleTestUri( "listElements" ), mc );
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        Resource head;

        head = mc.castToResource( stack.getFirst() );
        stack = stack.getRest();

        Iterator<Value> iter = mc.listValue( head ).iterator();
        while ( iter.hasNext() )
            sink.put( new RippleStack( iter.next(), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
