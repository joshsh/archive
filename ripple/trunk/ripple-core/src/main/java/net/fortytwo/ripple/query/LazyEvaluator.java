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
import net.fortytwo.ripple.model.StackRelation;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;
import org.apache.log4j.Logger;

// Note: not thread-safe, on account of stop()
public class LazyEvaluator extends StackEvaluator
{
	private static final Logger LOGGER = Logger.getLogger( LazyEvaluator.class );

	private boolean stopped = true;

	////////////////////////////////////////////////////////////////////////////

	protected class RelationSink implements Sink<StackContext>
	{
		private StackRelation relation;
		private Sink<StackContext> sink;

		public RelationSink( final StackRelation relation, final Sink<StackContext> sink )
		{
			this.relation = relation;
			this.sink = sink;
//System.out.println( this + "( " + relation + ", " + sink + ")" );
//System.out.println( "relation.arity() = " + relation.arity() );
		}

		public void put( final StackContext arg ) throws RippleException
		{
			if ( stopped )
			{
				return;
			}

//System.out.println( this + ".put( " + stack + " )" );
//System.out.println( "   first = " + stack.getFirst() );
			if ( relation.arity() == 1 )
			{
				relation.applyTo( arg, sink );
			}

			else
			{
				RippleList stack = arg.getStack();
				RippleValue first = stack.getFirst();
				RippleList rest = stack.getRest();

				Closure c = new Closure( relation, first );

				sink.put( arg.with( rest.push( new Operator( c ) ) ) );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	protected class EvaluatorSink implements Sink<StackContext>
	{
		private Sink<StackContext> sink;

		public EvaluatorSink( final Sink<StackContext> sink )
		{
			this.sink = sink;
//System.out.println( this + "( " + sink + ")" );
		}

		public void put( final StackContext arg )
			throws RippleException
		{
			if ( stopped )
			{
				return;
			}

			RippleList stack = arg.getStack();
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

				StackRelation f = ( (Operator) first ).getRelation();
//LOGGER.info( "   f = " + f );
//LOGGER.info( "   f.arity() = " + f.arity() );

				// Nullary functions don't need their argument stacks reduced.
				// They shouldn't even care if the stack is empty.
				if ( f.arity() == 0 )
				{
					f.applyTo( arg.with( rest ), this );
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
							new RelationSink( f, this ) ) ).put( arg.with( rest ) );
					}
				}
			}

			else
			{
				sink.put( arg );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void applyTo( final StackContext arg,
						final Sink<StackContext> sink )
		throws RippleException
	{
if ( RippleList.NIL == arg.getStack() )
{
	return;
}

		EvaluatorSink evalSink = new EvaluatorSink( sink );
		stopped = false;

		try
		{
			evalSink.put( arg );
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
