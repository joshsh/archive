package wurfel;

import java.util.Collection;


public class ApplyNode extends NullaryNode
{
    private Collection<Node> function, argument;

    public ApplyNode( final Collection<Node> function, final Collection<Node> argument )
    {
        this.function = function;
        this.argument = argument;
    }

    public Collection<Node> getFunction()
    {
        return function;
    }

    public Collection<Node> getArgument()
    {
        return argument;
    }

    public String toString()
    {
        return "@" + Node.toString( argument ) + " " + Node.toString( function );
    }

/*
    Collection<Node> reduce( ReductionContext context )
    {
        
    }
*/
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
