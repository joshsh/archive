package wurfel.model;

import wurfel.Context;

public abstract class NullaryNode extends Node
{
    public int arity()
    {
        return 0;
    }

    public Node execute( Node args[], Context context )
        throws Exception
    {
        throw new Exception( "can't apply an argument to a nullary node" );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
