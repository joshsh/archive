package wurfel.model;

import org.openrdf.model.impl.BNodeImpl;

public abstract class Node extends BNodeImpl
{
    public Node()
    {
        super( "no id" );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
