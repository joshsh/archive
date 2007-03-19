package net.fortytwo.ripple.model;

import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
    import net.fortytwo.ripple.model.filter.Unique;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class EagerStackEvaluator extends Evaluator
{
private URI uniqueFilterUri;

    private Model model;
    private ModelConnection modelConnection;

    ////////////////////////////////////////////////////////////////////////////

    protected class FunctionSink implements Sink<RippleStack>
    {
        private Function function;
        Sink<RippleStack> sink;

        public FunctionSink( Function function, Sink<RippleStack> sink )
        {
            this.function = function;
            this.sink = sink;
//System.out.println( this + "( " + function + ", " + sink + ")" );
//System.out.println( "function.arity() = " + function.arity() );
        }

        public void put( RippleStack stack )
            throws RippleException
        {
////System.out.println( this + ".put( " + stack + " )" );
////System.out.println( "   first = " + stack.getFirst() );
            if ( function.arity() == 1 )
                function.applyTo( stack, sink, modelConnection );

            else
            {
                Value first = stack.getFirst();
                RippleStack rest = stack.getRest();

                Closure c = new Closure( function, first );

                sink.put( new RippleStack( Operator.createOperator( c ), rest ) );
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    protected class EvaluatorSink implements Sink<RippleStack>
    {
        Sink<RippleStack> sink;

        public EvaluatorSink( Sink<RippleStack> sink )
        {
           this.sink = sink;
//System.out.println( this + "( " + sink + ")" );
        }

        public void put( RippleStack stack )
            throws RippleException
        {
//System.out.println( this + ".put( " + stack + " )" );
            Value first = stack.getFirst();
//System.out.println( "   first = " + stack.getFirst() );



            // prim[] and pred[]
            if ( first.isOperator() )
            {
                RippleStack rest = stack.getRest();

                if ( null == rest )
                    return;
//                    sink.put( stack );

                else
                    ( new EvaluatorSink(
                        new FunctionSink( ((Operator) first).getFunction(), this ) ) ).put( rest );
            }

            // arg[]
            else
                sink.put( stack );
        }
    }

    public void applyTo( RippleStack stack,
                        Sink<RippleStack> sink,
                        ModelConnection mc )
        throws RippleException
    {
//System.out.println( "public void applyTo" );
uniqueFilterUri = mc.createUri( "http://fortytwo.net/2007/03/04/rpl-new#unique" );
        modelConnection = mc;
        model = modelConnection.getModel();

        EvaluatorSink evalSink = new EvaluatorSink( sink );
        evalSink.put( stack );
    }

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
