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

    boolean isFunctionEnvelope( Value v )
    {
        return ( v instanceof FunctionEnvelope );
    }


    ////////////////////////////////////////////////////////////////////////////


    protected class FunctionSink implements Sink<ListNode<Value>>
    {
        private Function function;
        Sink<ListNode<Value>> sink;

        public FunctionSink( Function function, Sink<ListNode<Value>> sink )
        {
            this.function = function;
            this.sink = sink;
//System.out.println( this + "( " + function + ", " + sink + ")" );
//System.out.println( "function.arity() = " + function.arity() );
        }

        public void put( ListNode<Value> stack )
            throws RippleException
        {
////System.out.println( this + ".put( " + stack + " )" );
////System.out.println( "   first = " + stack.getFirst() );
            if ( function.arity() == 1 )
                function.applyTo( stack, sink, modelConnection );

            else
            {
                Value first = stack.getFirst();
                ListNode<Value> rest = stack.getRest();

                Closure c = new Closure( function, first );

                sink.put( new ListNode<Value>( FunctionEnvelope.createEnvelope( c ), rest ) );
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////////


    protected class EvaluatorSink implements Sink<ListNode<Value>>
    {
        Sink<ListNode<Value>> sink;

        public EvaluatorSink( Sink<ListNode<Value>> sink )
        {
           this.sink = sink;
//System.out.println( this + "( " + sink + ")" );
        }

        public void put( ListNode<Value> stack )
            throws RippleException
        {
//System.out.println( this + ".put( " + stack + " )" );
            Value first = stack.getFirst();
//System.out.println( "   first = " + stack.getFirst() );

            // prim[] and pred[]
            if ( isFunctionEnvelope( first ) )
            {
                ListNode<Value> rest = stack.getRest();

                if ( null == rest )
                    return;
//                    sink.put( stack );

                else
                    ( new EvaluatorSink( new FunctionSink( ((FunctionEnvelope) first).getFunction(), this ) ) ).put( rest );
            }

            // arg[]
            else
                sink.put( stack );
        }
    }

    public void reduce( ListNode<Value> stack,
                        Sink<ListNode<Value>> sink,
                        ModelConnection mc )
        throws RippleException
    {
//System.out.println( "public void reduce" );
uniqueFilterUri = mc.createUri( "http://fortytwo.net/2007/03/04/rpl-new#unique" );
        modelConnection = mc;
        model = modelConnection.getModel();

        EvaluatorSink evalSink = new EvaluatorSink( sink );
        evalSink.put( stack );
    }

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
