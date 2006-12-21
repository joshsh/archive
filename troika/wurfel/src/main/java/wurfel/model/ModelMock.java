package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.repository.Repository;
import org.openrdf.repository.Connection;
//import org.openrdf.sail.SailConnection;
import org.openrdf.util.iterator.CloseableIterator;
import org.openrdf.sail.SailException;
import org.openrdf.sail.Namespace;

import jline.Completor;
import jline.SimpleCompletor;
import jline.NullCompletor;

import java.util.Set;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;

public class ModelMock extends Model
{
    private Hashtable<String, URI> dictionary = null;
    private Hashtable<String, String> nsDictionary = null;

    private void createDictionaries()
        throws WurfelException
    {
        Set<URI> allURIs = new HashSet<URI>();
        dictionary = new Hashtable<String, URI>();
        nsDictionary = new Hashtable<String, String>();

        try
        {
            Connection conn = repository.getConnection();

            boolean includeInferred = true;
            CloseableIterator<? extends Statement> stmtIter
                = conn.getStatements(
//                    null, null, null, context, includeInferred );
                    null, null, null, includeInferred );
            while ( stmtIter.hasNext() )
            {
                Statement st = stmtIter.next();

                Resource subj = st.getSubject();
                URI pred = st.getPredicate();
                Value obj = st.getObject();

                if ( subj instanceof URI )
                    allURIs.add( (URI) subj );
                allURIs.add( pred );
                if ( obj instanceof URI )
                    allURIs.add( (URI) obj );
            }
            stmtIter.close();

            CloseableIterator<? extends Namespace> nsIter
                 = conn.getNamespaces();
            while ( nsIter.hasNext() )
            {
                Namespace ns = nsIter.next();
                nsDictionary.put( ns.getName(), ns.getPrefix() );
            }
            nsIter.close();

            conn.close();
        }

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        Iterator<URI> uriIter = allURIs.iterator();
        while ( uriIter.hasNext() )
        {
            URI uri = uriIter.next();
            dictionary.put( uri.getLocalName(), uri );
        }
    }

    public Completor getCompletor()
        throws WurfelException
    {
        if ( null == dictionary )
            createDictionaries();

        Set<String> dictKeys = dictionary.keySet();

        if ( dictKeys.size() > 0 )
        {
            String [] dictArray = dictKeys.toArray( new String[dictKeys.size()] );
            SimpleCompletor dictionaryCompletor = new SimpleCompletor( dictArray );
            return dictionaryCompletor;
        }

        else
            return new NullCompletor();
    }

    public URI resolve( final String name )
        throws WurfelException
    {
        if ( null == dictionary )
            createDictionaries();

        return dictionary.get( name );
    }

    public String nsPrefixOf( final URI uri )
        throws WurfelException
    {
        if ( null == nsDictionary )
            createDictionaries();

        return nsDictionary.get( uri.getNamespace() );
    }

    public ModelMock( Repository repository, Resource context )
    {
        super( repository, context );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
