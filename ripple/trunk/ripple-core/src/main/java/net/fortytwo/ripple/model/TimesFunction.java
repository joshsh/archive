package net.fortytwo.ripple.model;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Jan 15, 2008
 * Time: 9:24:59 PM
 */
public class TimesFunction implements Function
{
	private Function innerFunction;
	private int times;
	private boolean optional;

	public TimesFunction( final Function f, final int n, final boolean optional )
	{
		innerFunction = f;
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
		return innerFunction.isTransparent();
	}

	public void applyTo( final RippleList stack,
						 final Sink<RippleList> sink,
						 final ModelConnection mc ) throws RippleException
	{
		if ( 1 < times )
		{
			if ( optional )
			{
				sink.put( stack );
			}

			sink.put( stack
					.push( new Operator( innerFunction ) )
					.push( new Operator( new TimesFunction( innerFunction, times - 1, optional ) ) ) );
		}

		else if ( 1 == times )
		{
			if ( optional )
			{
				sink.put( stack );
			}

			sink.put( stack.push( new Operator( innerFunction ) ) );
		}

		// Note: we will not reach this condition unless the original
		// integer argument was less than 1.
		else
		{
			sink.put( stack );
		}
	}
}
