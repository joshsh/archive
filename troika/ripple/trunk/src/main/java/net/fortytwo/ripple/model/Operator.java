package net.fortytwo.ripple.model;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.vocabulary.RDF;

public class Operator implements RippleValue
{
    Function func;

    public Operator( RdfValue pred )
    {
        func = new ForwardPredicateQuery( pred );
    }

    public Operator( Function function )
    {
        func = function;
    }

    public Operator( RippleStack list )
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

    /**
     *  Guesses the type of a node and creates an appropriate wrapper.
     */
    public static Operator createOperator( RippleValue v, ModelConnection mc )
        throws RippleException
    {
        if ( v instanceof Function )
            return new Operator( (Function) v );

        else if ( v instanceof RippleStack )
            return new Operator( (RippleStack) v );

        // This is the messy part.  Attempt to guess the type of the object from
        // the available RDF statements, and create the appropriate object.
        if ( v instanceof RdfValue )
        {
// TODO: do some type inference instead of a dumb, single type lookup.
            RdfValue type = mc.findUniqueProduct( (RdfValue) v, rdfType );
            if ( type.getRdfValue().equals( RDF.PROPERTY ) )
                return new Operator( (RdfValue) v );
            else if ( type.getRdfValue().equals( RDF.LIST ) )
                return new Operator( new RippleStack( (RdfValue) v, mc ) );
            else
                throw new RippleException( "bad type in Operator createEnvelope(): "
                    + type );
        }

        else
            throw new RippleException( "bad RippleValue in Operator createEnvelope(): "
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

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
