package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

/**
 *  Carries out normal-order lazy beta reduction, distributing operations over
 *  node sets.
 */
public class LazyEvaluator extends Evaluator
{
    public Container reduce( Value expr, ModelConnection mc )
        throws WurfelException
    {
        if ( isApply( expr ) && ( (Apply) expr ).arity() == 0 )
        {
            // Reduce the function.
            Iterator<Value> reducedFuncIter = reduce(
                ( (Apply) expr ).getFunction(), mc ).iterator();

            // Reduce the argument.
            Iterator<Value> reducedArgIter = reduce(
                ( (Apply) expr ).getArgument(), mc ).iterator();

            // Iterate over the cartesian product of the reduced function(s)
            // with the reduced argument(s).
            LinkedList<Value> argList = new LinkedList<Value>();
            Container result = new Container();
            while ( reducedFuncIter.hasNext() )
            {
                Value function = reducedFuncIter.next();
                while ( reducedArgIter.hasNext() )
                {
                    Value argument = reducedArgIter.next();

                    // Apply the function to the argument.
                    Apply tmpApply = new Apply( function, argument );
                    if ( tmpApply.arity() == 0 )
                    {
                        // Argument list is initially empty.
                        argList.clear();

                        Collection<Value> itmResult = tmpApply.applyTo( argList, mc );

                        // Reduction is recursive; we must first iterate over
                        // the intermediate results and reduce them before
                        // adding them to the list of final results.
                        Iterator<Value> itmIter = itmResult.iterator();
                        while ( itmIter.hasNext() )
                            result.add( reduce( itmIter.next(), mc ) );
                    }

                    else
                        result.add( tmpApply );
                }
            }

            return result;
        }

        else
            return new Container( expr );
    }

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
