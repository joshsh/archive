package net.fortytwo.ripple.query;

import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.StackMapping;
import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Feb 11, 2008
 * Time: 3:15:35 PM
 */
public abstract class StackEvaluator extends Evaluator<StackContext, StackContext, RippleException> implements StackMapping
{
	public int arity()
	{
		return 1;
	}
}
