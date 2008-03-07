package net.fortytwo.ripple.query;

import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.model.StackMapping;

/**
 * Author: josh
 * Date: Feb 11, 2008
 * Time: 3:15:35 PM
 */
public abstract class StackEvaluator extends Evaluator<StackContext, StackContext> implements StackMapping
{
	public int arity()
	{
		return 1;
	}
}
