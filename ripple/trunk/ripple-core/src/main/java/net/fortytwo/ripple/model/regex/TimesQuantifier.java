package net.fortytwo.ripple.model.regex;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.StackRelation;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.Operator;

/**
 * Author: josh
 * Date: Jan 15, 2008
 * Time: 9:24:59 PM
 */
public class TimesQuantifier implements StackRelation
{
	private Operator innerOperator;
	private int min, max;

	public TimesQuantifier( final Operator oper, final int min, final int max )
	{
		this.innerOperator = oper;
		this.min = min;
		this.max = max;
	}

	public int arity()
	{
		// TODO
		return 1;
	}

	public boolean isTransparent()
	{
		return innerOperator.getRelation().isTransparent();
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink ) throws RippleException
	{
		RippleList stack = arg.getStack();

		if ( 0 == min )
		{
			sink.put( arg );
		}

		if ( max > 0 )
		{
			if ( 1 == max )
			{
				sink.put( arg.with( stack.push( innerOperator ) ) );
			}

			else
			{
				int newMin = ( 0 == min) ? 0 : min - 1, newMax = max - 1;

				sink.put( arg.with( stack
						.push( innerOperator )
						.push(new Operator( new TimesQuantifier( innerOperator, newMin, newMax ) ) ) ) );

			}
		}
	}
}
