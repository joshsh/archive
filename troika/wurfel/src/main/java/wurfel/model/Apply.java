package wurfel.model;

import wurfel.Wurfel;
import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.model.URI;
import org.openrdf.model.Literal;
import org.openrdf.model.Value;
import org.openrdf.sesame.sail.StatementIterator;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

// TODO: subclass LiteralImpl instead of implementing Literal
public class Apply extends Node implements Function
{
//FIXME
    public URI getUri()
{
return null;
}

    private static final URI
        s_applyClassUri = Wurfel.getWurfelUri( "Apply" );

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

    public Collection<Value> applyTo( LinkedList<Value> args, Context context )
        throws WurfelException
    {
// TODO: this is a temporary check
checkArguments( args );

        if ( function instanceof Function )
        {
            args.addFirst( argument );
            return ( (Function) function ).applyTo( args, context );
        }

        else
        {
            return context.apply( function, argument );
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



    public String toString()
    {
//        String s = ".";
        String s = "";

        if ( null == function )
            s += "()";
        else
            s += function.toString();

        s += " ";

        if ( null == argument )
            s += "()";
        else if ( argument instanceof Apply )
            s += "(" + argument.toString() + ")";
        else
            s += argument.toString();

        return s;
    }

}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
