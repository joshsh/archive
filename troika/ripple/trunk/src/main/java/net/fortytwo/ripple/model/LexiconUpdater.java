package net.fortytwo.ripple.model;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;

import org.openrdf.model.Namespace;
import org.openrdf.model.Statement;

public class LexiconUpdater implements RdfSink
{
	Lexicon lexicon;
	RdfSink sink;
	boolean override;

	public LexiconUpdater( final Lexicon lexicon, final RdfSink sink )
	{
		this.lexicon = lexicon;
		this.sink = sink;

		override = Ripple.preferNewestNamespaceDefinitions();
	}

	public void put( final Statement st ) throws RippleException
	{
		lexicon.add( st );
		sink.put( st );
	}

	public void put( final Namespace ns ) throws RippleException
	{
		lexicon.add( ns, override );
		sink.put( ns );
	}

	public void put( final String comment ) throws RippleException
	{
		sink.put( comment );
	}
}

// kate: tab-width 4
