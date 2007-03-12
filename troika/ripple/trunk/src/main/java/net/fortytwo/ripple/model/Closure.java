package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class Closure implements Function
{
    private Function function;
    private Value argument;
    private int cachedArity;

    public Closure( Function function, Value argument )
    {
        this.function = function;
        this.argument = argument;
        cachedArity = function.arity() - 1;
//System.out.println( "" + this + ": (" + function + ", " + argument + ")" );
    }

    ////////////////////////////////////////////////////////////////////////////

public URI getUri()
{
return null;
}

    public int arity()
    {
        return cachedArity;
    }

public void checkArguments( ListNode<Value> args )
    throws RippleException
{}

public void printTo( RipplePrintStream p )
    throws RippleException
{}

public Value toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

    ////////////////////////////////////////////////////////////////////////////

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
        function.applyTo( new ListNode<Value>( argument, stack ), sink,  mc );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
