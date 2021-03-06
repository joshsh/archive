package net.fortytwo.ripple.model.regex;

import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.StackMapping;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.StackContext;

/**
 * Author: josh
 * Date: Feb 14, 2008
 * Time: 4:30:14 PM
 */
public class OptionalQuantifier implements StackMapping
{
	private Operator innerOperator;

	public OptionalQuantifier( final Operator oper )
	{
		innerOperator = oper;
	}

	public int arity()
	{
		// TODO
		return 1;
	}

	public boolean isTransparent()
	{
		return innerOperator.getMapping().isTransparent();
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink ) throws RippleException
	{
		sink.put( arg );

		sink.put( arg.with( arg.getStack()
				.push( innerOperator ) ) );
	}
}
