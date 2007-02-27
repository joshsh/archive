package ripple;

import org.openrdf.model.Value;

import wurfel.model.Container;
import wurfel.model.Node;
//import wurfel.model.Function;

public class Stack extends Node //implements Function
{
    private Value first;
    private Stack rest;

// FIXME: probably not necessary
    private int depth;

    public Value getFirst()
    {
        return first;
    }

    public Value getRest()
    {
        return rest;
    }

    public int getDepth()
    {
        return depth;
    }

    public Stack( final Value first )
    {
        this.first = first;
        rest = null;

        depth = 1;
    }

    public Stack( final Value first, final Stack rest )
    {
        this.first = first;
        this.rest = rest;

        if ( null == rest )
            depth = 1;
        else
            this.depth = rest.depth + 1;
    }
}



/*

public class PropertySink extends Sink<Stack>
{
    URI property;
    Sink<Stack> sink;

    public PropertySink( URI property, Sink<Stack> sink )
    {
        this.property = property;
        this.sink = sink;
    }

    public void put( Stack stack )
        throws WurfelException
    {
        Value first = stack.getFirst();
        Stack rest = stack.getRest();

        Iterator<Value> objects = model.rdfMultiply( first, property ).iterator();
        while ( objects.hasNext() )
            sink.put( new Stack( objects.next(), rest );
    }
}



// TODO: use Sink<Value> for Primitives and Combinators


public class FunctionSink extends Sink<Stack>
{
    private Sink<Stack> sink;

    private Function function;

    // Note: this Stack serves a different purpose than an expression Stack.
    private Stack arguments;

    private int arity;

    public FunctionSink( Function function, Sink<Stack> sink )
    {
        this.function = function;
        this.sink = sink;
        this.arguments = null;
        arity = function.arity;
    }

    public FunctionSink( FunctionSink other, Value arg )
    {
        this.function = other.function;
        this.sink = other.sink;
        this.arguments = new Stack( arg, other.arguments );
        this.arity = other.arity - 1;
    }

    public void put( Stack stack )
        throws WurfelException
    {
        Value first = stack.getFirst();
        Stack rest = stack.getRest();

        if ( arity == 1 )
        {
            reduce( rest, 

            Stack allArgs = new Stack( first, arguments );
            function.applyTo( allArgs, sink );
        }

        else
            reduce( rest, new FunctionSink( this, first ) );
    }
}



public class ApplySink
{
    Sink<Stack> sink;

    public void put( Stack stack )
    {
        Value first = stack.getFirst();

        if ( isFunction( first ) )
        {
            Stack argStack = stack.getRest();

            if ( null == argStack )
                sink.put( stack );

            else
                reduce( argStack, new FunctionSink( (Function) first, sink );
        }

        else if ( isStack( first ) )
        {

        }

        else if ( isProperty( first ) )
        {
            Stack argStack = stack.getRest();

            if ( null == argStack )
                sink.put( stack );

            else
                reduce( argStack, new PropertySink( (URI) first, sink ) );
        }

        else
            throw new WurfelException( "value cannot be applied: " + first.toString() );
    }
}



    if ( first.equals( rplApplyUri ) )
        reduce( stack.getRest(), new ApplySink( sink ) );




    ApplyCallback applyCallback;


    public void apply()
    {

    }

    public class ApplyCallback
    {
        public void consume( Stack stack, Container out )
        {
            Value first = stack.getFirst();
    
            if ( first.equals( rplApplyUri ) )
            {
            out.add(
        }
    }


    applyCallback = new ApplyCallback();


boolean isCombinator( Value v )
{
    return ( v instanceof Combinator );
}

boolean isPrimitive( Value v )
{
    return ( v instanceof Primitive );
}

boolean isStack( Value v )
{
    return ( v instanceof Stack );
}

boolean isProperty( Value v )
{
    return ( v instanceof URI );
}


void apply( Stack stack, Sink<Value> sink )
{
    Value first = stack.getFirst();
    stack = stack.getRest();

    if ( first.equals( rplApplyUri ) )
    {
    sink.put(

    else
    {


        else if ( isProperty( first ) )
        {

        }


    }
}


Collection<Stack> evaluate( List<Value> list )
{
    Iterator<Value> iter = list.iterator();
    Stack stack = new Stack( list.next() );
    while ( list.hasNext() )
        stack = new Stack( list.next(), stack );

    Collection<Stack> result;

    if ( stack.getFirst().equals( rplApplyUri ) )
        result = apply( stack.getRest() );

    else
    {
        result = new ArrayList<Stack>();
        result.add( stack );
    }

    return result;
}




//*/







// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
