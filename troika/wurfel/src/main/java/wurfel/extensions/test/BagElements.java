package wurfel.extensions.test;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Resource;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class BagElements extends PrimitiveFunction
{
    public BagElements( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "bagElements" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        Resource head;

        Iterator<Value> argIter = args.iterator();
        head = mc.castToResource( argIter.next() );

        return mc.bagValue( head );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
