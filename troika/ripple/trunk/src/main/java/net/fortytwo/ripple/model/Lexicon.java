package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.jline.LexicalCompletor;

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

public class Lexicon extends Observable
{
	private Model model;

	private Hashtable<String, List<URI>> keywordsToUrisMap = null;
	private Hashtable<URI, String> urisToKeywordsMap = null;
	private Hashtable<String, String> prefixToNamespaceMap = null;
	private Hashtable<String, String> namespaceToPrefixMap = null;
	private Collection<String> qNamesCollection = null;

	public Lexicon( Model model )
		throws RippleException
	{
		this.model = model;

		// Create (immutable) keywords map.
		{
			keywordsToUrisMap = new Hashtable<String, List<URI>>();
			urisToKeywordsMap = new Hashtable<URI, String>();

			Collection<Value> keySet = model.getBridge().keySet();
			for ( Iterator<Value> keyIter = keySet.iterator(); keyIter.hasNext(); )
			{
				Value v = keyIter.next();
				if ( v instanceof URI )
				{
					String keyword = ( (URI) v ).getLocalName();

					List<URI> siblings = keywordsToUrisMap.get( keyword );
			
					if ( null == siblings )
					{
						siblings = new ArrayList<URI>();
						keywordsToUrisMap.put( keyword, siblings );

						urisToKeywordsMap.put( (URI) v, keyword );
					}

					siblings.add( (URI) v );
				}
			}
		}

		updateMaps();
	}

	public List<URI> resolveKeyword( final String localName )
	{
		List<URI> result = keywordsToUrisMap.get( localName );

		// If there are no results, return an empty list instead of null.
		return ( null == result )
			? new ArrayList<URI>()
			: result;
	}

	/**
	*  Note: <code>nsPrefix</code> must be the prefix of a defined namespace.
	*  <code>localName</code> is unconstrained.
	*/
	public URI resolveQName( final String nsPrefix,
							final String localName )
	{
		String ns = resolveNamespacePrefix( nsPrefix );

		return ( null == ns )
			? null
			: model.getRepository().getValueFactory().createURI( ns, localName );
	}

	public String resolveNamespacePrefix( final String nsPrefix )
	{
		return prefixToNamespaceMap.get( nsPrefix );
	}

	public String symbolFor( final URI uri )
	{
		// Does it have a keyword?
		String symbol = urisToKeywordsMap.get( uri );

		// If not, does it have a namespace prefix?
		if ( null == symbol )
		{
			String nsPrefix = namespaceToPrefixMap.get( uri.getNamespace() );

			// Namespace prefix may be empty but non-null.
			if ( null != nsPrefix )
				// Note: assumes that the local name is never null (although it
				//       may be empty).
				symbol = nsPrefix + ":" + uri.getLocalName();
		}

		return symbol;
	}

	public String nsPrefixOf( final URI uri )
	{
		return namespaceToPrefixMap.get( uri.getNamespace() );
	}

	public Completor getCompletor()
		throws RippleException
	{
		Set<String> localNames = keywordsToUrisMap.keySet();
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

	public void add( final Statement st )
		throws RippleException
	{
		Resource subj = st.getSubject();
		URI pred = st.getPredicate();
		Value obj = st.getObject();

		if ( subj instanceof URI )
			add( (URI) subj );
		add( pred );
		if ( obj instanceof URI )
			add( (URI) obj );
	}

	public void add( final Namespace ns )
	{
//System.out.println( "(" + ns.getPrefix() + "=" + ns.getName() + ")" );
		prefixToNamespaceMap.put( ns.getPrefix(), ns.getName() );
		namespaceToPrefixMap.put( ns.getName(), ns.getPrefix() );
	}

	// Note: assumes that the same URI will not be added twice.
	private void add( URI uri )
		throws RippleException
	{
		// If possible, add a qualified name as well.
		String prefix = nsPrefixOf( uri );
		if ( null != prefix )
		{
			String qName = prefix + ":" + uri.getLocalName();
			qNamesCollection.add( qName );
		}
	}

	private void updateMaps()
		throws RippleException
	{
		prefixToNamespaceMap = new Hashtable<String, String>();
		namespaceToPrefixMap = new Hashtable<String, String>();
		qNamesCollection = new ArrayList<String>();

		ModelConnection mc = model.getConnection( "for Lexicon update()" );

		try
		{
			RepositoryResult<Statement> stmtIter
				= mc.getRepositoryConnection().getStatements(
//                    null, null, null, model, includeInferred );
					null, null, null, Ripple.useInference() );
			while ( stmtIter.hasNext() )
				add( stmtIter.next() );
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
	}

	////////////////////////////////////////////////////////////////////////////

	// Public access allows the Interpreter to force a refresh.
	public void update()
		throws RippleException
	{
		updateMaps();

		setChanged();
		notifyObservers();
	}
}

// kate: tab-width 4
