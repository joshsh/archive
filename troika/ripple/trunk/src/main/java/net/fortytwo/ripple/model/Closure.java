package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;

public class Closure implements Function
{
    private Function innerFunction;
    private RippleValue argument;
    private int cachedArity;

    private RdfValue rdfEquivalent = null;

    public Closure( Function innerFunction, RippleValue argument )
    {
        this.innerFunction = innerFunction;
        this.argument = argument;
        cachedArity = innerFunction.arity() - 1;
//System.out.println( "" + this + ": (" + innerFunction + ", " + argument + ")" );
    }

    public int arity()
    {
        return cachedArity;
    }

// TODO: does not indicate application
    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        select ( Ripple.getExpressionOrder() )
        {
            case DIAGRAMMATIC:
                p.print( innerFunction );
                p.print( " " );
                p.print( argument );
                break;

            case ANTIDIAGRAMMATIC:
                p.print( argument );
                p.print( " " );
                p.print( innerFunction );
                break;
        }
    }

    public Value toRdf( ModelConnection mc )
        throws RippleException
    {
        if ( null == rdfEquivalent )
        {
            rdfEquivalent = mc.createBNode();

            mc.add( rdfEquivalent, 
        }

        return rdfEquivalent;
    }

    public void applyTo( RippleStack stack,
                         Sink<RippleStack> sink,
                         ModelConnection mc )
        throws RippleException
    {
        innerFunction.applyTo( new RippleStack( argument, stack ), sink,  mc );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
