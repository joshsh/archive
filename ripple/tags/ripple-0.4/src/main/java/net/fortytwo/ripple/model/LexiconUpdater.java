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
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

/**
 * Note: several LexiconUpdaters may safely be attached to a single Lexicon.
 */
public class LexiconUpdater implements RdfDiffSink
{
// TODO: Unicode characters supported by the lexer / Turtle grammar
	private static final Pattern PREFIX_PATTERN
		= Pattern.compile( "[A-Za-z][-0-9A-Z_a-z]*" );

	private RdfSink addSink, subSink;

	public LexiconUpdater( final Lexicon lexicon )
	{
		final boolean override = Ripple.preferNewestNamespaceDefinitions();
		final boolean allowDuplicateNamespaces = Ripple.allowDuplicateNamespaces();

		addSink = new RdfSink()
		{
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
					Resource subj = st.getSubject();
					URI pred = st.getPredicate();
					Value obj = st.getObject();
			
					synchronized ( lexicon )
					{
						if ( subj instanceof URI )
						{
							lexicon.add( (URI) subj );
						}
				
						lexicon.add( pred );
				
						if ( obj instanceof URI )
						{
							lexicon.add( (URI) obj );
						}
					}
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
					if ( !allowedNsPrefix( ns.getPrefix() ) )
					{
						return;
					}
				
					synchronized ( lexicon )
					{
						if ( override || null == lexicon.resolveNamespacePrefix( ns.getPrefix() ) )
						{
							if ( allowDuplicateNamespaces || null == lexicon.nsPrefixOf( ns.getName() ) )
							{
								lexicon.add( ns );
							}
						}
					}
				}
			};

			private Sink<String> cmtSink = new Sink<String>()
			{
				public void put( final String comment ) throws RippleException
				{
				}
			};

			public Sink<Statement> statementSink()
			{
				return stSink;
			}
		
			public Sink<Namespace> namespaceSink()
			{
				return nsSink;
			}
		
			public Sink<String> commentSink()
			{
				return cmtSink;
			}
		};

// TODO
		subSink = new RdfSink()
		{
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
				}
			};

			private Sink<String> cmtSink = new Sink<String>()
			{
				public void put( final String comment ) throws RippleException
				{
				}
			};

			public Sink<Statement> statementSink()
			{
				return stSink;
			}
		
			public Sink<Namespace> namespaceSink()
			{
				return nsSink;
			}
		
			public Sink<String> commentSink()
			{
				return cmtSink;
			}
		};
	}

	public RdfSink adderSink()
	{
		return addSink;
	}

	public RdfSink subtractorSink()
	{
		return subSink;
	}

	private boolean allowedNsPrefix( final String nsPrefix )
	{
		return PREFIX_PATTERN.matcher( nsPrefix ).matches();
	}
}

// kate: tab-width 4
