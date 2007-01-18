package wurfel.model;

import wurfel.WurfelException;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.repository.Repository;
import org.openrdf.repository.Connection;
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
import java.util.Observable;
import java.util.Observer;

public class Lexicon extends Observable implements Observer
{
    private Model model;

    private Hashtable<String, URI> dictionary = null;
    private Hashtable<String, String> nsDictionary = null;

    public Lexicon( Model model )
        throws WurfelException
    {
        this.model = model;
        model.addObserver( this );

        refresh();
    }

    // TODO: this should throw an exception if the identifier does not resolve
    //       to a unique value.
    public URI resolve( final String name )
        throws WurfelException
    {
        return dictionary.get( name );
    }

    public String nsPrefixOf( final URI uri )
        throws WurfelException
    {
        return nsDictionary.get( uri.getNamespace() );
    }

    public Completor getCompletor()
        throws WurfelException
    {
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

    ////////////////////////////////////////////////////////////////////////////

    private void refresh()
        throws WurfelException
    {
        Connection conn = model.getConnection();

System.out.println( "################# Rebuilding dictionaries." );
        Set<URI> allURIs = new HashSet<URI>();
        dictionary = new Hashtable<String, URI>();
        nsDictionary = new Hashtable<String, String>();

        try
        {
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

        setChanged();
        notifyObservers();
    }

    public void update( Observable o, Object arg )
    {
        try
        {
            if ( o == model )
                refresh();
        }

        catch ( WurfelException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
