package wurfel.model;


public class TypeNode extends NullaryNode
{
    private String name;
    private Class javaClass;

    public TypeNode( final String name,
                     final Class javaClass )
    {
        this.name = name;
        this.javaClass = javaClass;
    }

    public String getName()
    {
        return name;
    }

    public Class getJavaClass()
    {
        return javaClass;
    }

    public String toString()
    {
        return "<" + name + ">";
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
