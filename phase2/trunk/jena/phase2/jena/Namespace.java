package phase2.jena;

import java.util.Iterator;
import java.util.Hashtable;

////////////////////////////////////////////////////////////////////////////////


public class Namespace extends Hashtable<String, Object>
{
    public Namespace()
    {
        super();
    }

    private static String removeProtocol( String s )
    {
        int i = s.lastIndexOf( "://" );

        if ( i >= 0 )
            s = s.substring( i + 3 );

        return s;
    }

    private static String removeResource( String s )
    {
        int i = s.indexOf( "#" );

        if ( i >= 0 )
            s = s.substring( 0, i );

        return s;
    }

    private Namespace createChild( String id )
    {
        Object child = get( id );

        if ( child == null )
        {
            child = new Namespace();
            put( id, child );
        }

        else if ( !( child instanceof Namespace ) )
        {
            System.err.println( "Non-namespace object already exists with name \"" + id + "\"" );
            child = null;
        }

        return ( Namespace ) child;
    }

    public void createChildren( String s )
    {
        if ( s == null || s.length() == 0 || s.charAt( 0 ) == '-' )
            return;

        // Redundant, since we're using recursion.
        s = removeProtocol( s );
        s = removeResource( s );

        String id;

        int i = s.indexOf( "/" );

        if ( i >= 0 )
        {
            id = s.substring( 0, i );
            s = s.substring( i + 1 );
        }

        else
        {
            id = s;
            s = null;
        }

        Namespace child = createChild( id );

        if ( child != null )
            child.createChildren( s );
    }

    public void writeScript( String path )
    {
        if ( path == null )
            path = "";
        else if ( path.length() > 0 )
            path += ":";

        Iterator<String> iter = keySet().iterator();

        while ( iter.hasNext() )
        {
            String id = iter.next();
            Object o = get( id );

            if ( o instanceof Namespace )
            {
                String localPath = path + id;
                System.out.println( "_new " + localPath + ";" );
                ( ( Namespace ) o ).writeScript( localPath );
            }
        }
    }
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
