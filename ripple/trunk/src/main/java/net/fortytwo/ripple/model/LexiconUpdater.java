/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.util.regex.Pattern;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.io.RdfSink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class LexiconUpdater implements RdfSink
{
// TODO: Unicode characters supported by the lexer / Turtle grammar
	Pattern prefixPattern = Pattern.compile( "[A-Za-z][-0-9A-Z_a-z]*" );

	Lexicon lexicon;
	RdfSink sink;
	boolean override;
	boolean allowDuplicateNamespaces;

	public LexiconUpdater( final Lexicon lexicon, final RdfSink sink )
	{
		this.lexicon = lexicon;
		this.sink = sink;

		override = Ripple.preferNewestNamespaceDefinitions();
		allowDuplicateNamespaces = Ripple.allowDuplicateNamespaces();
	}

	public void put( final Statement st ) throws RippleException
	{
		Resource subj = st.getSubject();
		URI pred = st.getPredicate();
		Value obj = st.getObject();

		if ( subj instanceof URI )
			lexicon.add( (URI) subj );
		lexicon.add( pred );
		if ( obj instanceof URI )
			lexicon.add( (URI) obj );

		sink.put( st );
	}

	public void put( final Namespace ns ) throws RippleException
	{
		if ( !allowedNsPrefix( ns.getPrefix() ) )
			return;

		if ( override || null == lexicon.resolveNamespacePrefix( ns.getPrefix() ) )
		{
			if ( allowDuplicateNamespaces || null == lexicon.nsPrefixOf( ns.getName() ) )
			{
				lexicon.add( ns );
				sink.put( ns );
			}
		}
	}

	public void put( final String comment ) throws RippleException
	{
		sink.put( comment );
	}

	boolean allowedNsPrefix( final String nsPrefix )
	{
		return prefixPattern.matcher( nsPrefix ).matches();
	}
}

// kate: tab-width 4
