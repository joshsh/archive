/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.jline.LexicalCompletor;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;
import org.openrdf.model.URI;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;

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

public class Lexicon
{
	ValueFactory valueFactory;

	Hashtable<String, List<URI>> keywordsToUrisMap = null;
	Hashtable<URI, String> urisToKeywordsMap = null;
	Hashtable<String, String> prefixToNamespaceMap = null;
	Hashtable<String, String> namespaceToPrefixMap = null;
	Collection<String> qNamesCollection = null;

	public Lexicon( final Model model )
		throws RippleException
	{
		valueFactory = model.getRepository().getValueFactory();
		ModelConnection mc = model.getConnection( "for Lexicon constructor" );

		// Create (immutable) keywords map.
		{
			keywordsToUrisMap = new Hashtable<String, List<URI>>();
			urisToKeywordsMap = new Hashtable<URI, String>();

			ModelBridge bridge = model.getBridge();
			Iterator<Value> keys = bridge.keySet().iterator();
			while ( keys.hasNext() )
			{
				// An extra trip through the bridge replaces aliases with
				// "definitive" values.
				Value v = bridge.get( keys.next() ).toRdf( mc ).getRdfValue();

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

					// The presence of aliases will cause the same URI / keyword
					// pair to appear more than once.
					if ( !siblings.contains( (URI) v ) )
						siblings.add( (URI) v );
				}
			}
		}

		mc.close();

		prefixToNamespaceMap = new Hashtable<String, String>();
		namespaceToPrefixMap = new Hashtable<String, String>();
		qNamesCollection = new ArrayList<String>();
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
			: valueFactory.createURI( ns, localName );
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

	public String nsPrefixOf( final String uri )
	{
		return namespaceToPrefixMap.get( uri );
	}

	public Completor getCompletor()
		throws RippleException
	{
		Set<String> keywords = keywordsToUrisMap.keySet();
		Set<String> prefixes = prefixToNamespaceMap.keySet();

		int size = keywords.size() + prefixes.size() + qNamesCollection.size();
		if ( 0 < size )
		{
			Collection<String> alts = new ArrayList<String>();

			Iterator<String> localNameIter = keywords.iterator();
			while ( localNameIter.hasNext() )
				alts.add( localNameIter.next() );

			Iterator<String> qNameIter = qNamesCollection.iterator();
			while( qNameIter.hasNext() )
				alts.add( qNameIter.next() );

			Iterator<String> prefixIter = prefixes.iterator();
			while ( prefixIter.hasNext() )
				alts.add( prefixIter.next() + ":" );

			return new LexicalCompletor( alts );
		}

		else
			return new NullCompletor();
	}

	////////////////////////////////////////////////////////////////////////////

	public void add( final Namespace ns )
	{
//System.out.println( "(" + ns.getPrefix() + "=" + ns.getName() + ")" );
		prefixToNamespaceMap.put( ns.getPrefix(), ns.getName() );
		namespaceToPrefixMap.put( ns.getName(), ns.getPrefix() );
	}

	// Note: assumes that the same URI will not be added twice.
	public void add( URI uri )
		throws RippleException
	{
//System.out.println( "adding URI: " + uri );
		// If possible, add a qualified name as well.
		String prefix = namespaceToPrefixMap.get( uri.getNamespace() );
		if ( null != prefix )
		{
			String qName = prefix + ":" + uri.getLocalName();
//System.out.println( "adding qname: " + qName );
			qNamesCollection.add( qName );
		}
	}
}

// kate: tab-width 4
