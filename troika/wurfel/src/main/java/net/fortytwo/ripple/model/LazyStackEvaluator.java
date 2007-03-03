package net.fortytwo.ripple.model;

import java.util.Iterator;

import wurfel.WurfelException;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class LazyStackEvaluator extends Evaluator
{
    private Model model;
    private ModelConnection modelConnection;
    private URI applyOp;


    boolean isApplyOp( Value v )
    {
        return v.equals( applyOp );
    }

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
System.out.println( "public PropertySink -- " + property );
System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
System.out.println( "( (PropertySink) " + property + " ).put()" );
System.out.flush();
            Value first = stack.getFirst();
            ListNode<Value> rest = stack.getRest();

            Iterator<Value> objects = model.multiply( first, property, modelConnection ).iterator();
            while ( objects.hasNext() )
                if ( null == rest )
                    sink.put( new ListNode<Value>( objects.next() ) );
                else
                    sink.put( rest.push( objects.next() ) );
        }
    }


    protected class FunctionSink implements Sink<ListNode<Value>>
    {
        private Function function;
        Sink<ListNode<Value>> sink;

        public FunctionSink( Function function, Sink<ListNode<Value>> sink )
        {
            this.function = function;
            this.sink = sink;
System.out.println( "public FunctionSink( " + function + ", " + sink + ")" );
System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
            if ( function.arity() == 1 )
                function.applyTo( stack, sink, modelConnection );

            else
            {
                Value first = stack.getFirst();
                ListNode<Value> rest = stack.getRest();

                Closure c = new Closure( function, first );

                sink.put( new ListNode<Value>( c, rest ) );
            }
        }
    }

/*
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
System.out.println( "public FunctionSink( Function function, Sink<ListNode<Value>> sink ) -- " + function + ", " + arity );
System.out.flush();
        }

        public FunctionSink( FunctionSink other, Value first )
        {
            function = other.function;
            sink = other.sink;
            arity = other.arity - 1;
            arguments = new ListNode<Value>( first, other.arguments );
System.out.println( "public FunctionSink( FunctionSink other, Value first ) -- " + function + ", " + arity );
System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
System.out.println( "( (FunctionSink) " + this + " ).put() -- " + function + ", " + arity );
System.out.flush();
            Value first = stack.getFirst();
System.out.println( "first = " + first );
System.out.flush();
            ListNode<Value> rest = stack.getRest();

            if ( 1 == arity )
            {
// TODO: this work should only be done once, not once for each application.
                ListNode<Value> args = arguments;
                while ( null != args )
                {
System.out.println( "move arg" );
System.out.flush();
                    stack = stack.push( args.getFirst() );
                    args = args.getRest();
                }
//                stack = stack.push( first );

                function.applyTo( stack, sink, modelConnection );
            }

            else
                sink.put( new ListNode<Value>( new FunctionSink(
//                evalSink.put( rest, new FunctionSink( this, first ) );
        }
    }
*/


    protected class ApplySink implements Sink<ListNode<Value>>
    {
        Sink<ListNode<Value>> sink;

        public ApplySink( Sink<ListNode<Value>> sink )
        {
            this.sink = sink;
System.out.println( "public ApplySink" );
System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
System.out.println( "( (ApplySink) " + this + " ).put()" );
System.out.flush();
//        if ( null == stack )
//            return;

            Value first = stack.getFirst();

            if ( isApplyOp( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    return;
/*
                {
                    stack = new ListNode<Value>( first, stack );
                    sink.put( stack );
                }
*/
                else
                    ( new EvaluatorSink( new ApplySink( sink ) ) ).put( stack.getRest() );
            }

            else if ( isFunction( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    return;
//                    sink.put( stack );

                else
                    ( new EvaluatorSink( new FunctionSink( (Function) first, sink ) ) ).put( rest );
            }

            else if ( isList( first ) )
            {
// ...
            }

            else if ( isProperty( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    return;
//                    sink.put( stack );

                else
                    ( new EvaluatorSink( new PropertySink( (URI) first, sink ) ) ).put( rest );
            }

            else
                throw new WurfelException( "value cannot be applied: " + first.toString() );
        }
    }


    ////////////////////////////////////////////////////////////////////////////


// TODO: merge this with ApplySink, if possible.
    protected class EvaluatorSink implements Sink<ListNode<Value>>
    {
        Sink<ListNode<Value>> sink;

        public EvaluatorSink( Sink<ListNode<Value>> sink )
        {
            this.sink = sink;
        }

        public void put( ListNode<Value> stack )
            throws WurfelException
        {
System.out.println( "EvaluatorSink put()" );
System.out.flush();
            Value first = stack.getFirst();
System.out.println( "   first = " + first );
System.out.flush();

            if ( isApplyOp( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    return;
//                    sink.put( stack );

                else
                    ( new EvaluatorSink( new ApplySink( this ) ) ).put( stack.getRest() );
            }

            else
                sink.put( stack );
        }
    }

    public void reduce( ListNode<Value> stack,
                        Sink<ListNode<Value>> sink,
                        ModelConnection mc )
        throws WurfelException
    {
System.out.println( "public void reduce" );
System.out.flush();
        modelConnection = mc;
        model = modelConnection.getModel();
        applyOp = mc.getApplyOp();

        EvaluatorSink evalSink = new EvaluatorSink( sink );
        evalSink.put( stack );
    }

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
