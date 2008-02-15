package net.fortytwo.ripple.model.regex;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.StackRelation;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.StackContext;

/**
 * Author: josh
 * Date: Feb 14, 2008
 * Time: 4:28:41 PM
 */
public class StarQuantifier implements StackRelation
{
	private Operator innerOperator;

	public StarQuantifier( final Operator oper )
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
		return innerOperator.getRelation().isTransparent();
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink ) throws RippleException
	{
		sink.put( arg );

		sink.put( arg.with( arg.getStack()
				.push( innerOperator )
				.push( new Operator( this ) ) ) );
	}
}
