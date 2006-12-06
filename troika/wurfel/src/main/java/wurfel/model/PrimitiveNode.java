package wurfel.model;

import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;


// TODO: write a factory class.
public abstract class PrimitiveNode extends Node
{
    class Param
    {
        public String name;
        public TypeNode type;
        public String annotation;
        public boolean transparency;

        public Param( final String name,
                      final TypeNode type,
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
    private TypeNode returnType;
    private String name;
    private String annotation;

    public PrimitiveNode( final String name,
                          final TypeNode returnType,
                          final String annotation )
    {
        this.name = name;
        params = new ArrayList<Param>();
        this.returnType = returnType;
        this.annotation = annotation;
    }

    public String getName()
    {
        return name;
    }

    public TypeNode getReturnType()
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

    public void addParameter( final String name,
                              final TypeNode type,
                              final String annotation,
                              final boolean transparency )
    {
        Param p = new Param( name, type, annotation, transparency );
        params.add( p );
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

    public TypeNode getParameterType( final int index )
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
        String s = Node.toString( returnType ) + " " + name + "(";
        boolean first = true;
        Iterator<Param> iter = params.iterator();
        while( iter.hasNext() )
        {
            if ( first )
                first = false;
            else
                s += ", ";

            Param p = iter.next();
            s += Node.toString( p.type ) + " " + p.name;
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
