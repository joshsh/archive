package wurfel.model;

import java.util.Collection;
import java.util.LinkedHashSet;

import org.openrdf.model.Value;

public class Container extends LinkedHashSet<Value>
{
    public Container()
    {
        super();
    }

    public Container( Value v )
    {
        super();

        add( v );
    }

    public Container( Collection<Value> other )
    {
        super();

        add( other );
    }

/*
    public boolean add( Value v )
    {
        return ( null == v ) ?
            false : ( (LinkedHashSet<Value>) this ).add( v );
    }
*/

    public boolean add( Collection<Value> other )
    {
        return addAll( other );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
