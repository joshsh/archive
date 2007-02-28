package net.fortytwo.ripple.model;

import java.util.Iterator;

import wurfel.WurfelException;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class LazyStackEvaluator
{
    private Model model;
    private ModelConnection modelConnection;
    private URI applyOp;


    boolean isFunction( Value v )
    {
        return ( v instanceof Function );
    }

    boolean isProperty( Value v )
    {
        return ( v instanceof URI );
    }

    boolean isList( Value v )
    {
        return ( v instanceof ListNode );
    }


    ////////////////////////////////////////////////////////////////////////////


    public class PropertySink implements Sink<ListNode<Value>>
    {
        URI property;
        Sink<ListNode<Value>> sink;

        public PropertySink( URI property, Sink<ListNode<Value>> sink )
        {
            this.property = property;
            this.sink = sink;
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
            Value first = stack.getFirst();
            ListNode<Value> rest = stack.getRest();

            Iterator<Value> objects = model.multiply( first, property, modelConnection ).iterator();
            while ( objects.hasNext() )
                sink.put( rest.push( objects.next() ) );
        }
    }


    protected class FunctionSink implements Sink<ListNode<Value>>
    {
        private Sink<ListNode<Value>> sink;

        private Function function;
        private ListNode<Value> arguments;
        private int arity;

        public FunctionSink( Function function, Sink<ListNode<Value>> sink )
        {
            this.function = function;
            this.sink = sink;
            arity = function.arity();
            this.arguments = null;
        }

        public FunctionSink( FunctionSink other, Value first )
        {
            function = other.function;
            sink = other.sink;
            arity = other.arity - 1;
            arguments = new ListNode<Value>( first, other.arguments );
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
            Value first = stack.getFirst();
            ListNode<Value> rest = stack.getRest();

            if ( 1 == arity )
            {
// TODO: this work should only be done once, not once for each application.
                while ( null != arguments )
                {
                    ListNode<Value> args = arguments;
                    stack = stack.push( args.getFirst() );
                    args = args.getRest();
                }

                function.applyTo( stack, sink, modelConnection );
            }

            else
                reduce( rest, new FunctionSink( this, first ) );
        }
    }


    protected class ApplySink implements Sink<ListNode<Value>>
    {
        Sink<ListNode<Value>> sink;

        public ApplySink( Sink<ListNode<Value>> sink )
        {
            this.sink = sink;
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
//        if ( null == stack )
//            return;

            Value first = stack.getFirst();

            if ( first.equals( applyOp ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                {
                    stack = new ListNode<Value>( first, stack );
                    sink.put( stack );
                }

                else
                    reduce( stack.getRest(), new ApplySink( sink ) );
            }

            else if ( isFunction( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    sink.put( stack );

                else
                    reduce( rest, new FunctionSink( (Function) first, sink ) );
            }

            else if ( isList( first ) )
            {
// ...
            }

            else if ( isProperty( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    sink.put( stack );

                else
                    reduce( rest, new PropertySink( (URI) first, sink ) );
            }

            else
                throw new WurfelException( "value cannot be applied: " + first.toString() );
        }
    }


    ////////////////////////////////////////////////////////////////////////////


    public void reduce( ListNode<Value> stack,
                        Sink<ListNode<Value>> sink,
                        ModelConnection mc )
        throws WurfelException
    {
        modelConnection = mc;
        model = modelConnection.getModel();
        applyOp = mc.getApplyOp();

        reduce( stack, sink );
    }

// TODO: merge reduce() with ApplySink, if possible.
// TODO: make reduce() into the put() of a Sink.
    protected void reduce( ListNode<Value> stack, Sink<ListNode<Value>> sink )
        throws WurfelException
    {
        Value first = stack.getFirst();

        if ( first.equals( applyOp ) )
        {
            ListNode<Value> rest = stack.getRest();

            if ( null == rest )
                sink.put( stack );
            else
                reduce( stack.getRest(), new ApplySink( sink ) );
        }

        else
            sink.put( stack );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
