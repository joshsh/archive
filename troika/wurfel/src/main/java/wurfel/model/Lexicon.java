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

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Observable;
import java.util.Observer;

public class Lexicon extends Observable implements Observer
{
    private Model model;

    private Hashtable<String, List<URI>> localNameToUrisMap = null;
    private Hashtable<String, String> prefixToNamespaceMap = null;
    private Hashtable<String, String> namespaceToPrefixMap = null;

    public Lexicon( Model model )
        throws WurfelException
    {
        this.model = model;
        model.addObserver( this );

        refresh();
    }

    public List<URI> resolveUnqualifiedName( final String localName )
    {
        return localNameToUrisMap.get( localName );
    }

    public URI resolveQualifiedName( final String nsPrefix,
                                     final String localName )
    {
        String ns = resolveNamespacePrefix( nsPrefix );
        if ( null == ns )
            return null;

        Iterator<URI> uris = localNameToUrisMap.get( localName ).iterator();
        while ( uris.hasNext() )
        {
            URI uri = uris.next();
            if ( uri.getNamespace().equals( ns ) )
                return uri;
        }

        return null;
    }

    public String resolveNamespacePrefix( final String nsPrefix )
    {
        return prefixToNamespaceMap.get( nsPrefix );
    }

    public String nsPrefixOf( final URI uri )
        throws WurfelException
    {
        return namespaceToPrefixMap.get( uri.getNamespace() );
    }

    public Completor getCompletor()
        throws WurfelException
    {
        Set<String> localNames = localNameToUrisMap.keySet();
        Set<String> prefixes = prefixToNamespaceMap.keySet();

        int size = localNames.size() + prefixes.size();
        if ( 0 < size )
        {
            String [] alts = new String[size];
            int index = 0;

            Iterator<String> localNameIter = localNames.iterator();
            while ( localNameIter.hasNext() )
                alts[index++] = localNameIter.next();

            Iterator<String> prefixIter = prefixes.iterator();
            while ( prefixIter.hasNext() )
                alts[index++] = prefixIter.next() + ":";

            return new SimpleCompletor( alts );
        }

        else
            return new NullCompletor();
    }

    ////////////////////////////////////////////////////////////////////////////

    private void add( final Namespace ns )
    {
        prefixToNamespaceMap.put( ns.getPrefix(), ns.getName() );
        namespaceToPrefixMap.put( ns.getName(), ns.getPrefix() );
    }

    // Note: assumes that the same URI will not be added twice.
    private void add( URI uri )
    {
        String localName = uri.getLocalName();
        List<URI> siblings = localNameToUrisMap.get( localName );

        if ( null == siblings )
        {
            siblings = new ArrayList<URI>();
            localNameToUrisMap.put( localName, siblings );
        }

        siblings.add( uri );
    }

    private void refresh()
        throws WurfelException
    {
System.out.println( "################# Rebuilding dictionaries." );
        localNameToUrisMap = new Hashtable<String, List<URI>>();
        prefixToNamespaceMap = new Hashtable<String, String>();
        namespaceToPrefixMap = new Hashtable<String, String>();

        Set<URI> allURIs = new HashSet<URI>();

        Connection conn = model.getConnection();

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

            // Namespace prefixes are managed by OpenRDF, and are simply
            // imported into the Lexicon.
            CloseableIterator<? extends Namespace> nsIter
                 = conn.getNamespaces();
            while ( nsIter.hasNext() )
                add( nsIter.next() );
            nsIter.close();

            conn.close();
        }

        catch ( SailException e )
        {
            throw new WurfelException( e );
        }

        Iterator<URI> uriIter = allURIs.iterator();
        while ( uriIter.hasNext() )
            add( uriIter.next() );

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
