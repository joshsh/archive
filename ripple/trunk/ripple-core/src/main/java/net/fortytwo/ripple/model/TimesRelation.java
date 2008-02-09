package net.fortytwo.ripple.model;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Jan 15, 2008
 * Time: 9:24:59 PM
 */
public class TimesRelation implements StackRelation
{
	private StackRelation innerRelation;
	private int times;
	private boolean optional;

	public TimesRelation( final StackRelation f, final int n, final boolean optional )
	{
		innerRelation = f;
		times = n;
		this.optional = optional;
	}

	public int arity()
	{
		// TODO
		return 1;
	}

	public boolean isTransparent()
	{
		return innerRelation.isTransparent();
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	) throws RippleException
	{
		RippleList stack = arg.getStack();

		if ( 1 < times )
		{
			if ( optional )
			{
				sink.put( arg );
			}

			sink.put( arg.with( stack
					.push( new Operator( innerRelation ) )
					.push( new Operator( new TimesRelation( innerRelation, times - 1, optional ) ) ) ) );
		}

		else if ( 1 == times )
		{
			if ( optional )
			{
				sink.put( arg );
			}

			sink.put( arg.with( stack.push( new Operator( innerRelation ) ) ) );
		}

		// Note: we will not reach this condition unless the original
		// integer argument was less than 1.
		else
		{
			sink.put( arg );
		}
	}
}
