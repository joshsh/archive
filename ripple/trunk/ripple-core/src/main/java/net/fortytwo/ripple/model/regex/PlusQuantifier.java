package net.fortytwo.ripple.model.regex;

import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.StackRelation;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.regex.StarQuantifier;

/**
 * Author: josh
 * Date: Feb 14, 2008
 * Time: 4:31:15 PM
 */
public class PlusQuantifier implements StackRelation
{
	private Operator innerOperator;

	public PlusQuantifier( final Operator oper )
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
		sink.put( arg.with( arg.getStack()
				.push( innerOperator )
				.push( new Operator( new StarQuantifier( innerOperator ) ) ) ) );
	}
}

