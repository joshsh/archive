package wurfel.model;

import wurfel.Wurfel;
import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Apply extends Node implements Function
{
//FIXME
    public URI getUri()
{
return null;
}

    private Value function, argument;
//    private Collection<Value> function, argument;

    private int arityCached;
    public int arity()
    {
        return arityCached;
    }

    public void checkArguments( LinkedList<Value> args )
        throws WurfelException
    {
        if ( args.size() != arity() )
            throw new WurfelException( "attempt to apply a "
                + arity() + "-ary function to a list of "
                + args.size() + " arguments" );
    }

    public Collection<Value> applyTo( LinkedList<Value> args, EvaluationContext evalContext )
        throws WurfelException
    {
// TODO: this is a temporary check
checkArguments( args );

        if ( function instanceof Function )
        {
            args.addFirst( argument );
            return ( (Function) function ).applyTo( args, evalContext );
        }

        else
        {
            return evalContext.getContext().apply( function, argument, evalContext );
        }
    }

    public Apply( final Value function, final Value argument )
//    public Apply( final Collection<Value> function, final Collection<Value> argument )
    {
        this.function = function;
        this.argument = argument;

        if ( function instanceof Function )
            arityCached = ( (Function) function ).arity() - 1;
        else
            arityCached = 0;
    }

    public Value getFunction()
//    public Collection<Value> getFunction()
    {
        return function;
    }

    public Value getArgument()
//    public Collection<Value> getArgument()
    {
        return argument;
    }

    // Note: assumes left associativity for now.
    public String toString()
    {
        Value left = null, right = null;

        switch ( Wurfel.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                left = argument;
                right = function;
                break;

            case ANTIDIAGRAMMATIC:
                left = function;
                right = argument;
                break;
        }

        String leftStr = ( null == left )
            ? "()" : left.toString();
        String rightStr = ( null == right )
            ? "()" : ( right instanceof Apply )
                ? "(" + right.toString() + ")" : right.toString();

        return leftStr + " " + rightStr;
    }

    public void printTo( WurfelPrintStream p )
        throws WurfelException
    {
        Value left = null, right = null;

        switch ( Wurfel.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                left = argument;
                right = function;
                break;

            case ANTIDIAGRAMMATIC:
                left = function;
                right = argument;
                break;
        }

        p.print( left );
        p.print( " " );

        if ( null != right && right instanceof Apply )
        {
            p.print( "(" );
            p.print( right );
            p.print( ")" );
        }

        else
            p.print( right );
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
