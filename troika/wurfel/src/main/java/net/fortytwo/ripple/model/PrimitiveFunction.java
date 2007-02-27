package net.fortytwo.ripple.model;

import wurfel.Wurfel;
import wurfel.WurfelException;

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

    protected abstract Collection<Value> applyInternal( LinkedList<Value> args,
                                                        ModelConnection mc )
        throws WurfelException;

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

    private Param getWurfelParameter( final Resource head, ModelConnection mc )
        throws WurfelException
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
        throws WurfelException
    {
        selfUri = self;

// TODO: actually use these, or get rid of them
        name = "";
        annotation = "";

        wurfelParametersUri             = mc.createWurfelUri( "parameters" );
        wurfelReturnTypeUri             = mc.createWurfelUri( "returnType" );
        wurfelParameterUri              = mc.createWurfelUri( "Parameter" );
        wurfelParameterNameUri          = mc.createWurfelUri( "parameterName" );
        wurfelParameterListUri          = mc.createWurfelUri( "ParameterList" );
        wurfelParameterTypeUri          = mc.createWurfelUri( "parameterType" );
        wurfelParameterTransparencyUri  = mc.createWurfelUri( "parameterTransparency" );

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
                throw new WurfelException( "non-Resource encountered as an argument to a PrimitiveFunction" );
            else
                params.add( getWurfelParameter( (Resource) val, mc ) );
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

    public void checkArguments( LinkedList<Value> args )
        throws WurfelException
    {
        if ( args.size() != arity() )
            throw new WurfelException( "attempt to apply a "
                + arity() + "-ary function to a list of "
                + args.size() + " arguments" );
    }

    public Collection<Value> applyTo( LinkedList<Value> args, ModelConnection mc )
        throws WurfelException
    {
// TODO: this is a temporary check
checkArguments( args );

// TODO: type checking

        return applyInternal( args, mc );
    }

    public void printTo( WurfelPrintStream p )
        throws WurfelException
    {
        p.print( selfUri );
    }

    public Value toRdf( ModelConnection mc )
        throws WurfelException
    {
        return selfUri;
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
