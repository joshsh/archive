package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.BNode;
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

    public void checkArguments( ListNode<Value> args )
        throws RippleException
    {
/*
        if ( args.size() != arity() )
            throw new RippleException( "attempt to apply a "
                + arity() + "-ary function to a list of "
                + args.size() + " arguments" );
*/
    }

    private class ValueSinkToListSink implements Sink<Value>
    {
        private Sink<ListNode<Value>> sink;

        public ValueSinkToListSink( Sink<ListNode<Value>> sink )
        {
            this.sink = sink;
        }

        public void put( Value v ) throws RippleException
        {
            sink.put( new ListNode<Value>( v ) );
        }
    }

    public void applyTo( ListNode<Value> args,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
// TODO: this is a temporary check
checkArguments( args );

        if ( function instanceof Function )
            ( (Function) function ).applyTo(
                args.push( argument ), sink, mc );

        else
        {
            Iterator<Value> result = mc.getModel().multiply( argument, function, mc ).iterator();

            while ( result.hasNext() )
                sink.put( new ListNode( result.next() ) );
        }
    }

    public Apply( final Value function, final Value argument )
    {
        this.function = function;
        this.argument = argument;

        if ( function instanceof Function )
            arityCached = ( (Function) function ).arity() - 1;
        else
            arityCached = 0;
    }

    public Value getFunction()
    {
        return function;
    }

    public Value getArgument()
    {
        return argument;
    }

    // Note: assumes left associativity for now.
    public String toString()
    {
        Value left = null, right = null;

        switch ( Ripple.getExpressionOrder() )
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

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        Value left = null, right = null;

        switch ( Ripple.getExpressionOrder() )
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

    public Value toRdf( ModelConnection mc )
        throws RippleException
    {
        Value funcRdf = mc.toRdf( function );
        Value argRdf = mc.toRdf( argument );

        BNode selfRdf = mc.createBNode();
URI wurfelApplyFunctionUri = mc.createRippleUri( "applyFunction" );
URI wurfelApplyArgumentUri = mc.createRippleUri( "applyArgument" );

        mc.add( selfRdf, wurfelApplyFunctionUri, funcRdf );
        mc.add( selfRdf, wurfelApplyArgumentUri, argRdf );

        return selfRdf;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
