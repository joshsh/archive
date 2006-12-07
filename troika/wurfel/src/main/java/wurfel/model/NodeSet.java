package wurfel.model;

import org.openrdf.model.Value;

import java.util.LinkedHashSet;

public class NodeSet extends LinkedHashSet<Value>
{
    public NodeSet()
    {
        super();
    }

    public NodeSet( Value v )
    {
        super();

        add( v );
    }
/*
    public boolean add( Value v )
    {
        return ( null == v ) ?
            false : ( (LinkedHashSet<Value>) this ).add( v );
    }
*/
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
