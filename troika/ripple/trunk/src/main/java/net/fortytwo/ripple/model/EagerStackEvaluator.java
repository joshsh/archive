package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
    import net.fortytwo.ripple.model.filter.Unique;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class EagerStackEvaluator extends Evaluator
{
private URI uniqueFilterUri;

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

    boolean isFunctionEnvelope( Value v )
    {
        return ( v instanceof FunctionEnvelope );
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
System.out.println( this + "( " + property + ", " + sink + " )" );
//System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws RippleException
        {
System.out.println( this + ".put( " + stack + " )" );
//System.out.flush();
System.out.println( "   first = " + stack.getFirst() );
//System.out.flush();
            Value first = stack.getFirst();
            ListNode<Value> rest = stack.getRest();

            Iterator<Value> objects = model.multiply( first, property, modelConnection ).iterator();
            while ( objects.hasNext() )
                sink.put( new ListNode<Value>( objects.next(), rest ) );
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
System.out.println( this + "( " + function + ", " + sink + ")" );
System.out.println( "function.arity() = " + function.arity() );
//System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws RippleException
        {
//System.out.println( this + ".put( " + stack + " )" );
//System.out.flush();
//System.out.println( "   first = " + stack.getFirst() );
//System.out.flush();
            if ( function.arity() == 1 )
                function.applyTo( stack, sink, modelConnection );

            else
            {
                Value first = stack.getFirst();
                ListNode<Value> rest = stack.getRest();

                Closure c = new Closure( function, first );

                sink.put( new ListNode<Value>( new FunctionEnvelope( c ), rest ) );
            }
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
System.out.println( this + "( " + sink + ")" );
//System.out.flush();
        }

        public void put( ListNode<Value> stack )
            throws RippleException
        {
System.out.println( this + ".put( " + stack + " )" );
//System.out.flush();
            Value first = stack.getFirst();
System.out.println( "   first = " + stack.getFirst() );
//System.out.flush();

            if ( isFunctionEnvelope( first ) )
            {
                // prim[]
                if ( null != ( (FunctionEnvelope) first ).getFunction() )
                {
System.out.println( "prim[" + ( (FunctionEnvelope) first ).getFunction() + "]" );
                    ListNode<Value> rest = stack.getRest();
    
                    if ( null == rest )
                        return;
    //                    sink.put( stack );
    
                    else
                        ( new EvaluatorSink( new FunctionSink( ((FunctionEnvelope) first).getFunction(), this ) ) ).put( rest );
                }

                // pred[]
                else
                {
System.out.println( "pred[" + ( (FunctionEnvelope) first ).getPredicate() + "]" );
                    ListNode<Value> rest = stack.getRest();
    
                    if ( null == rest )
                        return;
    //                    sink.put( stack );
    
                    else
                        ( new EvaluatorSink( new PropertySink( ((FunctionEnvelope) first).getPredicate(), this ) ) ).put( rest );
                }
            }

            // arg[]
            else
                sink.put( stack );
/*
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
*/
        }
    }

    public void reduce( ListNode<Value> stack,
                        Sink<ListNode<Value>> sink,
                        ModelConnection mc )
        throws RippleException
    {
System.out.println( "public void reduce" );
//System.out.flush();
uniqueFilterUri = mc.createUri( "http://fortytwo.net/2007/03/04/rpl-new#unique" );
        modelConnection = mc;
        model = modelConnection.getModel();
        applyOp = mc.getApplyOp();

        EvaluatorSink evalSink = new EvaluatorSink( sink );
        evalSink.put( stack );
    }

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
