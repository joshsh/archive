package net.fortytwo.ripple.rdf.diff;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;

public final class RdfDiffContextFilter implements RdfDiffSink
{
	private Map<Resource, RdfDiffCollector> contextToCollectorMap;

	private RdfSink addSink = new RdfSink()
	{
		private Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				Resource context = st.getContext();

				RdfDiffCollector sink = contextToCollectorMap.get( context );
				if ( null == sink )
				{
					sink = new RdfDiffCollector();
					contextToCollectorMap.put( context, sink );
				}

				sink.adderSink().statementSink().put( st );
			}
		};

		private Sink<Namespace> nsSink = new Sink<Namespace>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				contextToCollectorMap.get( null ).adderSink().namespaceSink().put( ns );
			}
		};

		private Sink<String> comSink = new Sink<String>()
		{
			public void put( final String comment ) throws RippleException
			{
				contextToCollectorMap.get( null ).adderSink().commentSink().put( comment );
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
			return comSink;
		}
	};

	private RdfSink subSink = new RdfSink()
	{
		private Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				Resource context = st.getContext();

				RdfDiffCollector sink = contextToCollectorMap.get( context );
				if ( null == sink )
				{
					sink = new RdfDiffCollector();
					contextToCollectorMap.put( context, sink );
				}

				sink.subtractorSink().statementSink().put( st );
			}
		};

		private Sink<Namespace> nsSink = new Sink<Namespace>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				contextToCollectorMap.get( null ).subtractorSink().namespaceSink().put( ns );
			}
		};

		private Sink<String> comSink = new Sink<String>()
		{
			public void put( final String comment ) throws RippleException
			{
				contextToCollectorMap.get( null ).subtractorSink().commentSink().put( comment );
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
			return comSink;
		}
	};

	public RdfDiffContextFilter()
	{
		contextToCollectorMap = new HashMap<Resource, RdfDiffCollector>();

		clear();
	}

	public Iterator<Resource> contextIterator()
	{
		return contextToCollectorMap.keySet().iterator();
	}

	public void clear()
	{
		contextToCollectorMap.clear();

		RdfDiffCollector nullCollector = new RdfDiffCollector();
		contextToCollectorMap.put( null, nullCollector );
	}

	public void clearContext( final Resource context )
	{
		contextToCollectorMap.remove( context );
	}

	public RdfSink adderSink()
	{
		return addSink;
	}

	public RdfSink subtractorSink()
	{
		return subSink;
	}
}

// kate: tab-width 4
