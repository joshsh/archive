package wurfel.model;

import wurfel.Wurfel;
import wurfel.Context;
import wurfel.WurfelException;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

// TODO: write a factory class.
public abstract class PrimitiveFunction implements Function
{
    public abstract URI getUri();

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

    private static final URI
        s_wurfelPrimitiveFunctionUri = Wurfel.getWurfelUri( "PrimitiveFunction" ),
        s_wurfelParametersUri = Wurfel.getWurfelUri( "parameters" ),
        s_wurfelReturnTypeUri = Wurfel.getWurfelUri( "returnType" ),
        s_wurfelParameterUri = Wurfel.getWurfelUri( "Parameter" ),
        s_wurfelParameterNameUri = Wurfel.getWurfelUri( "parameterName" ),
        s_wurfelParameterListUri = Wurfel.getWurfelUri( "ParameterList" ),
        s_wurfelParameterTypeUri = Wurfel.getWurfelUri( "parameterType" ),
        s_wurfelParameterTransparencyUri = Wurfel.getWurfelUri( "parameterTransparency" );

    private Param getWurfelParameter( final Resource head, Context context )
        throws WurfelException
    {
        String name = context.getString( head, s_wurfelParameterNameUri );
        URI type = context.getUri( head, s_wurfelParameterTypeUri );

// TODO: actually use this, or get rid of it
        String annotation = "";

        boolean transparency = context.getBoolean( head, s_wurfelParameterTransparencyUri );

        return new Param( name, type, annotation, transparency );
    }

    public PrimitiveFunction( final URI self, Context context )
        throws WurfelException
    {
// TODO: actually use these, or get rid of them
        name = "";
        annotation = "";

        returnType = context.getUri( self, s_wurfelReturnTypeUri );
        Resource paramList = context.getResource( self, s_wurfelParametersUri );
        Iterator<Value> paramIter = context.getRdfList( paramList ).iterator();

        params = new ArrayList<Param>();

        while ( paramIter.hasNext() )
        {
            Value val = paramIter.next();
            if ( !( val instanceof Resource ) )
                throw new WurfelException( "non-Resource encountered as an argument to a PrimitiveFunction" );
            else
                params.add( getWurfelParameter( (Resource) val, context ) );
        }
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

    public int arity()
    {
        return params.size();
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
