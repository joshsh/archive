package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.jline.LexicalCompletor;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.model.Namespace;

import jline.Completor;
import jline.SimpleCompletor;
import jline.NullCompletor;

import java.util.ArrayList;
import java.util.Collection;
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
    private Collection<String> qNamesCollection = null;

    public Lexicon( Model model )
        throws RippleException
    {
        this.model = model;
        model.addObserver( this );

        createMaps();
    }

    public List<URI> resolveUnqualifiedName( final String localName )
    {
        return localNameToUrisMap.get( localName );
    }

    /**
     *  Note: <code>nsPrefix</code> must be the prefix of a defined namespace.
     *  <code>localName</code> is unconstrained.
     */
    public URI resolveQualifiedName( final String nsPrefix,
                                     final String localName )
    {
        String ns = resolveNamespacePrefix( nsPrefix );

        if ( null == ns )
            return null;

        else
        {
            return model.getRepository().getValueFactory().createURI( ns, localName );
        }
/*
        Iterator<URI> uris = localNameToUrisMap.get( localName ).iterator();
        while ( uris.hasNext() )
        {
            URI uri = uris.next();
            if ( uri.getNamespace().equals( ns ) )
                return uri;
        }

        return null;
*/
    }

    public String resolveNamespacePrefix( final String nsPrefix )
    {
        return prefixToNamespaceMap.get( nsPrefix );
    }

    public String nsPrefixOf( final URI uri )
        throws RippleException
    {
        return namespaceToPrefixMap.get( uri.getNamespace() );
    }

    public Completor getCompletor()
        throws RippleException
    {
        Set<String> localNames = localNameToUrisMap.keySet();
        Set<String> prefixes = prefixToNamespaceMap.keySet();

        int size = localNames.size() + prefixes.size() + qNamesCollection.size();
        if ( 0 < size )
        {
            Collection<String> alts = new ArrayList<String>();

            Iterator<String> localNameIter = localNames.iterator();
            while ( localNameIter.hasNext() )
                alts.add( localNameIter.next() );

            Iterator<String> prefixIter = prefixes.iterator();
            while ( prefixIter.hasNext() )
                alts.add( prefixIter.next() + ":" );

            Iterator<String> qNameIter = qNamesCollection.iterator();
            while( qNameIter.hasNext() )
                alts.add( qNameIter.next() );

            return new LexicalCompletor( alts );
//            return new SimpleCompletor( alts );
        }

        else
            return new NullCompletor();
    }

    ////////////////////////////////////////////////////////////////////////////

    private void add( final Namespace ns )
    {
//System.out.println( "(" + ns.getPrefix() + "=" + ns.getName() );
        prefixToNamespaceMap.put( ns.getPrefix(), ns.getName() );
        namespaceToPrefixMap.put( ns.getName(), ns.getPrefix() );
    }

    // Note: assumes that the same URI will not be added twice.
    private void add( URI uri )
        throws RippleException
    {
        String localName = uri.getLocalName();
        List<URI> siblings = localNameToUrisMap.get( localName );

        if ( null == siblings )
        {
            siblings = new ArrayList<URI>();
            localNameToUrisMap.put( localName, siblings );
        }

        siblings.add( uri );

        // If possible add a qualified name as well.
        String prefix = nsPrefixOf( uri );
        if ( null != prefix )
        {
            String qName = prefix + ":" + localName;
            qNamesCollection.add( qName );
        }
    }

    private void createMaps()
        throws RippleException
    {
System.out.println( "################# Rebuilding dictionaries." );
        localNameToUrisMap = new Hashtable<String, List<URI>>();
        prefixToNamespaceMap = new Hashtable<String, String>();
        namespaceToPrefixMap = new Hashtable<String, String>();
        qNamesCollection = new ArrayList<String>();

        Set<URI> allURIs = new HashSet<URI>();

        ModelConnection mc = new ModelConnection( model, "for Lexicon update()" );

        try
        {
            RepositoryResult<Statement> stmtIter
                = mc.getRepositoryConnection().getStatements(
//                    null, null, null, model, includeInferred );
                    null, null, null, Ripple.useInference() );
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
            RepositoryResult<Namespace> nsIter
                 = mc.getRepositoryConnection().getNamespaces();
            while ( nsIter.hasNext() )
                add( nsIter.next() );
            nsIter.close();
        }

        catch ( Throwable t )
        {
            mc.close();
            throw new RippleException( t );
        }

        mc.close();

        Iterator<URI> uriIter = allURIs.iterator();
        while ( uriIter.hasNext() )
            add( uriIter.next() );
    }

    ////////////////////////////////////////////////////////////////////////////

    private boolean changed = false;
    private boolean suspended = false;

    public synchronized void suspendEventHandling()
    {
        suspended = true;
    }

    public synchronized void resumeEventHandling()
        throws RippleException
    {
        if ( suspended )
        {
            if ( changed )
            {
                update();

                changed = false;
            }

            suspended = false;
        }
    }

    // Public access allows the Interpreter to force a refresh.
    public void update()
        throws RippleException
    {
        createMaps();

        setChanged();
        notifyObservers();
    }

    public void update( Observable o, Object arg )
    {
        try
        {
            if ( o == model )
            {
                if ( suspended )
                    changed = true;

                else
                    update();
            }
        }

        catch ( RippleException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
