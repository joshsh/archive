package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.ArrayList;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public abstract class PrimitiveFunction extends Node implements Function
{
    protected URI selfUri;

    protected abstract void applyInternal( ListNode<Value> args,
                                           Sink<ListNode<Value>> sink,
                                           ModelConnection mc )
        throws RippleException;

    class Param
    {
        public String name;
        public URI type;
        public String annotation;
        public boolean transparency;

        public Param( final String name,
                      final URI type,
                      final String annotation,
                      final boolean transparency )
        {
            this.name = name;
            this.type = type;
            this.annotation = annotation;
            this.transparency = transparency;
        }
    }

    private List<Param> params;
    private URI returnType;
    private String name;
    private String annotation;

    private URI
        wurfelParametersUri,
        wurfelReturnTypeUri,
        wurfelParameterUri,
        wurfelParameterNameUri,
        wurfelParameterListUri,
        wurfelParameterTypeUri,
        wurfelParameterTransparencyUri;

    private Param getRippleParameter( final Resource head, ModelConnection mc )
        throws RippleException
    {
        String name = mc.stringValue(
            mc.castToLiteral(
                mc.findUniqueProduct( head, wurfelParameterNameUri ) ) );
        URI type = mc.castToUri(
            mc.findUniqueProduct( head, wurfelParameterTypeUri ) );

// TODO: actually use this, or get rid of it
        String annotation = "";

        boolean transparency = mc.booleanValue(
            mc.castToLiteral(
                mc.findUniqueProduct( head, wurfelParameterTransparencyUri ) ) );

        return new Param( name, type, annotation, transparency );
    }

    public PrimitiveFunction( final URI self, ModelConnection mc )
        throws RippleException
    {
        selfUri = self;

// TODO: actually use these, or get rid of them
        name = "";
        annotation = "";

        wurfelParametersUri             = mc.createRippleUri( "parameters" );
        wurfelReturnTypeUri             = mc.createRippleUri( "returnType" );
        wurfelParameterUri              = mc.createRippleUri( "Parameter" );
        wurfelParameterNameUri          = mc.createRippleUri( "parameterName" );
        wurfelParameterListUri          = mc.createRippleUri( "ParameterList" );
        wurfelParameterTypeUri          = mc.createRippleUri( "parameterType" );
        wurfelParameterTransparencyUri  = mc.createRippleUri( "parameterTransparency" );

        returnType = mc.castToUri(
            mc.findUniqueProduct( self, wurfelReturnTypeUri ) );
        Resource paramList = mc.castToResource(
            mc.findUniqueProduct( self, wurfelParametersUri ) );
        Iterator<Value> paramIter = mc.listValue( paramList ).iterator();

        params = new ArrayList<Param>();

        while ( paramIter.hasNext() )
        {
            Value val = paramIter.next();
            if ( !( val instanceof Resource ) )
                throw new RippleException( "non-Resource encountered as an argument to a PrimitiveFunction" );
            else
                params.add( getRippleParameter( (Resource) val, mc ) );
        }
    }

    public URI getUri()
    {
        return selfUri;
    }


    public String getName()
    {
        return name;
    }

    public URI getReturnType()
    {
        return returnType;
    }

    public String getAnnotation()
    {
        return annotation;
    }

    public int parameters()
    {
        return params.size();
    }

    private Param getParam( final int index )
        throws Exception
    {
        if ( index < 0 || index >= params.size() )
            throw new Exception( "parameter index out of bounds: " + index );

        return params.get( index );
    }

    public String getParameterName( final int index )
        throws Exception
    {
        Param p = getParam( index );
        return p.name;
    }

    public URI getParameterType( final int index )
        throws Exception
    {
        Param p = getParam( index );
        return p.type;
    }

    public String getParameterAnnotation( final int index )
        throws Exception
    {
        Param p = getParam( index );
        return p.annotation;
    }

    public boolean getParameterTransparency( final int index )
        throws Exception
    {
        Param p = getParam( index );
        return p.transparency;
    }

    public String toString()
    {
        String s = returnType.toString() + " " + name + "(";
        boolean first = true;
        Iterator<Param> iter = params.iterator();
        while( iter.hasNext() )
        {
            if ( first )
                first = false;
            else
                s += ", ";

            Param p = iter.next();
            s += p.type.toString() + " " + p.name;
        }

        s += ")";
        return s;
    }

    ////////////////////////////////////////////////////////////////////////////

    public int arity()
    {
        return params.size();
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

    public void applyTo( ListNode<Value> args,
                         Sink<ListNode<Value>> sink,
                         ModelConnection mc )
        throws RippleException
    {
// TODO: this is a temporary check
checkArguments( args );

// TODO: type checking

        applyInternal( args, sink, mc );
    }

    public void printTo( RipplePrintStream p )
        throws RippleException
    {
        p.print( selfUri );
    }

    public Value toRdf( ModelConnection mc )
        throws RippleException
    {
        return selfUri;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
