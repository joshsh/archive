package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

public class LazyEvaluator extends Evaluator
{
	private Model model;
	private ModelConnection modelConnection;

	////////////////////////////////////////////////////////////////////////////

	protected class FunctionSink implements Sink<RippleList>
	{
		private Function function;
		Sink<RippleList> sink;

		public FunctionSink( Function function, Sink<RippleList> sink )
		{
			this.function = function;
			this.sink = sink;
//System.out.println( this + "( " + function + ", " + sink + ")" );
//System.out.println( "function.arity() = " + function.arity() );
		}

		public void put( RippleList stack )
			throws RippleException
		{
//System.out.println( this + ".put( " + stack + " )" );
//System.out.println( "   first = " + stack.getFirst() );
			if ( function.arity() == 1 )
				function.applyTo( stack, sink, modelConnection );

			else
			{
				RippleValue first = stack.getFirst();
				RippleList rest = stack.getRest();

				Closure c = new Closure( function, first );

				sink.put( new RippleList( new Operator( c ), rest ) );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	protected class EvaluatorSink implements Sink<RippleList>
	{
		Sink<RippleList> sink;

		public EvaluatorSink( Sink<RippleList> sink )
		{
		this.sink = sink;
//System.out.println( this + "( " + sink + ")" );
		}

		public void put( RippleList stack )
			throws RippleException
		{
//System.out.println( this + ".put( " + stack + " )" );
			RippleValue first = stack.getFirst();
//System.out.println( "   first = " + stack.getFirst() );

			// prim[], pred[], deq[]
			if ( first.isOperator() )
			{
				RippleList rest = stack.getRest();

				Function f = ((Operator) first).getFunction();

				// Nullary functions don't need their argument stacks reduced.
				// They shouldn't even care if the stack is null.
				if ( f.arity() == 0 )
					f.applyTo( rest, this, modelConnection );

				// Functions with positive arity do require the stack to be
				// reduced, to one level per argument.
				else
				{
					if ( null == rest )
						return;
//                    sink.put( stack );

					else
						( new EvaluatorSink(
							new FunctionSink( f, this ) ) ).put( rest );
				}
			}

			// arg[]
			else
				sink.put( stack );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
//System.out.println( "public void applyTo" );
		modelConnection = mc;
		model = modelConnection.getModel();

		EvaluatorSink evalSink = new EvaluatorSink( sink );
		evalSink.put( stack );
	}

}

// kate: tab-width 4
