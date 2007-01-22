package wurfel.model;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.Value;

/**
 *  A ... which reduces arbitrary expressions to a particular normal form.
 */
public abstract class Evaluator
{
    protected Context context;

    public Evaluator( Context context )
    {
        this.context = context;
    }

    public Context getContext()
    {
        return context;
    }

    protected static boolean isApply( Value v )
    {
        return v instanceof Apply;
    }

    public abstract NodeSet reduce( Value expr, EvaluationContext evalContext )
        throws WurfelException;
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on