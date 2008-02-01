/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Closure;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.Context;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

// Note: not thread-safe, on account of stop()
public class LazyEvaluator extends Evaluator
{
	private static final Logger LOGGER = Logger.getLogger( LazyEvaluator.class );

	private Context context;
	private boolean stopped = true;

	////////////////////////////////////////////////////////////////////////////

	protected class FunctionSink implements Sink<RippleList>
	{
		private Function function;
		private Sink<RippleList> sink;

		public FunctionSink( final Function function, final Sink<RippleList> sink )
		{
			this.function = function;
			this.sink = sink;
//System.out.println( this + "( " + function + ", " + sink + ")" );
//System.out.println( "function.arity() = " + function.arity() );
		}

		public void put( final RippleList stack )
			throws RippleException
		{
			if ( stopped )
			{
				return;
			}

//System.out.println( this + ".put( " + stack + " )" );
//System.out.println( "   first = " + stack.getFirst() );
			if ( function.arity() == 1 )
			{
				function.applyTo( stack, sink, context );
			}

			else
			{
				RippleValue first = stack.getFirst();
				RippleList rest = stack.getRest();

				Closure c = new Closure( function, first );

				sink.put( rest.push( new Operator( c ) ) );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	protected class EvaluatorSink implements Sink<RippleList>
	{
		private Sink<RippleList> sink;

		public EvaluatorSink( final Sink<RippleList> sink )
		{
			this.sink = sink;
//System.out.println( this + "( " + sink + ")" );
		}

		public void put( final RippleList stack )
			throws RippleException
		{
			if ( stopped )
			{
				return;
			}

//LOGGER.info( this + ".put( " + stack + " )" );
//System.out.println( this + ".put( " + stack + " )" );
			RippleValue first = stack.getFirst();
//LOGGER.info( "   first = " + stack.getFirst() );
//LOGGER.info( "   first.isActive() = " + first.isActive() );
//System.out.println( "   first = " + stack.getFirst() );

			if ( first.isActive() )
			{
				RippleList rest = stack.getRest();
//LOGGER.info( "   rest = " + rest );

				Function f = ( (Operator) first ).getFunction();
//LOGGER.info( "   f = " + f );
//LOGGER.info( "   f.arity() = " + f.arity() );

				// Nullary functions don't need their argument stacks reduced.
				// They shouldn't even care if the stack is empty.
				if ( f.arity() == 0 )
				{
					f.applyTo( rest, this, context );
				}

				// Functions with positive arity do require the stack to be
				// reduced, to one level per argument.
				else
				{
					// We simply ignore stacks which can't be reduced to
					// something with a passive item on top.
					if ( RippleList.NIL == rest )
					{
						return;
//						sink.put( stack );
					}

					else
					{
						( new EvaluatorSink(
							new FunctionSink( f, this ) ) ).put( rest );
					}
				}
			}

			else
			{
				sink.put( stack );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void applyTo( final RippleList stack,
						final Sink<RippleList> sink,
						final Context context )
		throws RippleException
	{
if ( stack == RippleList.NIL )
{
	return;
}

		this.context = context;

		EvaluatorSink evalSink = new EvaluatorSink( sink );
		stopped = false;

		try
		{
			evalSink.put( stack );
		}

		// Attempt to recover from stack overflow.
		catch ( StackOverflowError e )
		{
			throw new RippleException( e );
		}
	}

	public void stop()
	{
		synchronized ( this )
		{
			stopped = true;
		}
	}
}

// kate: tab-width 4
