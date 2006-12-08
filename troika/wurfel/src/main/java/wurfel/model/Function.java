package wurfel.model;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

import java.util.Collection;

public interface Function
{
//    public abstract String toString();

    public abstract int arity();
    public abstract Collection<Value> apply( Value args[], Context context )
        throws WurfelException;

/*
    public static String toString( Node n )
    {
        if ( null == n )
            return "()";
        else
            return n.toString();
    }

    public static String toString( Collection<Node> nodes )
    {
        if ( nodes.isEmpty() )
            return "()";
        else if ( nodes.size() == 1 )
            return nodes.iterator().next().toString();
        else
            return "{" + nodes.iterator().next().toString() + ", ...}";
    }
*/
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
