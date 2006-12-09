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


    /**
     *  Compares this object with the specified object for order. Returns a negative integer, zero, or a positive integer as this object is less than, equal to, or greater than the specified object.
     */
    public int compareTo( Object other )
    {
// TODO
        return -1;
    }

    /**
     *  Gets all statements from the RDF graph for which this value is the object.
     */
    public StatementIterator getObjectStatements()
    {
        return null;
    }

    /**
     *  Compares a literal object to another object.
     */
    public boolean equals( Object other )
    {
// TODO
        return false;
    }

    /**
     * Gets the datatype for this literal.
     */
    public URI getDatatype()
    {
        return s_applyClassUri;
    }

    /**
     *  Gets the label of this literal.
     */
    public String getLabel()
    {
// TODO
        return "Error: the value of an Apply is in its getFunction() and getArgument() members";
    }

    /**
     *  Gets the language tag for this literal, normalized to lowercase.
     */
    public String getLanguage()
    {
// TODO
        return "I don't know what to use for a language tag";
    }

    /**
     *  The hash code of a literal is defined as the hash code of its label: label.hashCode()
     */
    public int hashCode()
    {
        return getLabel().hashCode();
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

/*
//        String s = ".";
        String s = "";

        Iterator<Value> funcIter = function.iterator();
        if ( funcIter.hasNext() )
        {
            s += funcIter.next().toString();
            if ( funcIter.hasNext() )
                s += "[...]";
        }
        else
            s += "()";

        s += " ";

        Iterator<Value> argIter = argument.iterator();
        if ( argIter.hasNext() )
        {
            Value arg = argIter.next();
            if ( arg instanceof Apply )
                s += "(" + arg.toString() + ")";
            else
                s += arg.toString();
            if ( argIter.hasNext() )
                s += "[...]";
        }
        else
            s += "()";

        return s;
//        return "@" + Node.toString( argument ) + " " + Node.toString( function );
*/
    }

/*
    Collection<Node> reduce( ReductionContext context )
    {
        
    }
*/
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
