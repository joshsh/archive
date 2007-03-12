package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ListMemoizer;
import net.fortytwo.ripple.model.ListNode;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class FunctionEnvelope implements Function
{
    URI innerProperty = null;
    Function innerFunction = null;
//    ListNode<Value> innerList = null;

    public FunctionEnvelope( URI uri )
    {
        innerProperty = uri;
    }

    public FunctionEnvelope( Function function )
    {
        innerFunction = function;
    }

    public URI getPredicate()
    {
        return innerProperty;
    }

    public Function getFunction()
    {
        return innerFunction;
    }

    public int arity()
    {
        if ( null != innerProperty )
            return 1;

        else if ( null != innerFunction )
            return innerFunction.arity();

//        else
//            return 1;
else return 0;
    }

    public void applyTo( ListNode<Value> stack,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
/*
        if ( null != innerProperty )
        {
            Value first = stack.getFirst();
            ListNode<Value> rest = stack.getRest();

            Iterator<Value> objects = model.multiply( first, innerProperty, mc ).iterator();
            while ( objects.hasNext() )
                sink.put( new ListNode<Value>( objects.next(), rest ) );
        }

        else if ( null != innerFunction )
            innerFunction.applyTo( new ListNode<Value>( argument, stack ), sink,  mc );
*/

/*
        else
            ...
*/
    }


public URI getUri()
{
return null;
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

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
