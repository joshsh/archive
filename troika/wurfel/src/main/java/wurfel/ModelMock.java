package wurfel;

import org.openrdf.model.Graph;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.sesame.sail.StatementIterator;

import java.util.Set;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;

public class ModelMock extends Model
{
    Hashtable<String, URI> dictionary = null;
    private void createDictionary()
    {
        Set<URI> allURIs = new HashSet<URI>();
        dictionary = new Hashtable<String, URI>();

        StatementIterator stIter = graph.getStatements();
        while ( stIter.hasNext() )
        {
            Statement st = stIter.next();

            Resource subj = st.getSubject();
            URI pred = st.getPredicate();
            Value obj = st.getObject();

            if ( subj instanceof URI )
                allURIs.add( (URI) subj );
            allURIs.add( pred );
            if ( obj instanceof URI )
                allURIs.add( (URI) obj );
        }

        Iterator<URI> uriIter = allURIs.iterator();
        while ( uriIter.hasNext() )
        {
            URI uri = uriIter.next();
            dictionary.put( uri.getLocalName(), uri );
        }



System.out.println("Dictionary:");
Set<String> keys = dictionary.keySet();
Iterator<String> keyIter = keys.iterator();
while ( keyIter.hasNext() )
{
    String key = keyIter.next();
    System.out.println( "    " + key + ": " + dictionary.get( key ).toString() );
}

    }

    public URI resolve( final String name )
    {
        if ( null == dictionary )
            createDictionary();

        return dictionary.get( name );
    }

    public ModelMock( Graph g )
    {
        super( g );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
