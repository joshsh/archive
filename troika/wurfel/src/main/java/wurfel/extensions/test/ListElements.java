package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.ModelConnection;

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
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "listElements" ), mc );
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               ModelConnection mc )
        throws WurfelException
    {
        Resource head;

        Iterator<Value> argIter = args.iterator();
        head = mc.castToResource( argIter.next() );

        return new NodeSet( mc.listValue( head ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
