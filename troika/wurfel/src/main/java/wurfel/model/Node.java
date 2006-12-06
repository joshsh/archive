package wurfel.model;

import wurfel.Context;
import java.util.Collection;


public abstract class Node
{
    public abstract String toString();

    public abstract int arity();
    public abstract Node execute( Node args[], Context context )
        throws Exception;

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
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
