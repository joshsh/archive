package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.filter.Filter;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.vocabulary.RDF;

public class Operator implements RippleValue
{
    public static Operator
        PRIM = new Operator( (Function) null ),
        PRED = new Operator( (Function) null ),
        PROG = new Operator( (Function) null );

    Function func;

    public Operator( RdfValue pred )
    {
        func = new ForwardPredicateQuery( pred );
    }

    public Operator( Function function )
    {
        func = function;
    }

    public Operator( RippleList list )
    {
        func = new ListDequotation( list );
    }

    public Function getFunction()
    {
        return func;
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( "/" );
        p.print( func );
    }

    private static RdfValue
        rdfType = new RdfValue( RDF.TYPE );

    public static Operator guessOperator( RippleValue v, ModelConnection mc )
        throws RippleException
    {
        if ( v instanceof Function )
            return PRIM;

        else if ( v instanceof RippleList )
            return PROG;

        // This is the messy part.  Attempt to guess the type of the object from
        // the available RDF statements, and create the appropriate object.
        if ( v instanceof RdfValue )
        {
// TODO: do some type inference instead of a dumb, single type lookup.
            RdfValue type = mc.findAtMostOneObject( (RdfValue) v, rdfType );
            if ( null == type || type.getRdfValue().equals( RDF.PROPERTY ) )
                return PRED;
            else if ( type.getRdfValue().equals( RDF.LIST ) )
                return PROG;
            else
                throw new RippleException( "bad type in guessOperator(): "
                    + type );
        }

        else
            throw new RippleException( "bad RippleValue in guessOperator(): "
                + v );
    }

    /**
     *  Guesses the type of a node and creates an appropriate wrapper.
     */
    public static Operator createOperator( RippleValue v, ModelConnection mc )
        throws RippleException
    {
        if ( v instanceof Function )
            return new Operator( (Function) v );

        else if ( v instanceof RippleList )
            return new Operator( (RippleList) v );

        // This is the messy part.  Attempt to guess the type of the object from
        // the available RDF statements, and create the appropriate object.
        if ( v instanceof RdfValue )
        {
// TODO: do some type inference instead of a dumb, single type lookup.
            RdfValue type = mc.findAtMostOneObject( (RdfValue) v, rdfType );
            if ( null == type || type.getRdfValue().equals( RDF.PROPERTY ) )
                return new Operator( (RdfValue) v );
            else if ( type.getRdfValue().equals( RDF.LIST ) )
                return new Operator( new RippleList( (RdfValue) v, mc ) );
            else
                throw new RippleException( "bad type in createOperator(): "
                    + type );
        }

        else
            throw new RippleException( "bad RippleValue in createOperator(): "
                + v );
    }

    public boolean isOperator()
    {
        return true;
    }

public RdfValue toRdf( ModelConnection mc )
    throws RippleException
{
return null;
}

/*
    private class PrimFunc extends Filter
    {
        
    }
*/
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
