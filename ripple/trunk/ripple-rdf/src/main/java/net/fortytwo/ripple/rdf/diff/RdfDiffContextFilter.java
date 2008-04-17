/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.rdf.diff;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.rdf.RDFSink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;

public final class RdfDiffContextFilter implements RdfDiffSink
{
	private Map<Resource, RdfDiffCollector> contextToCollectorMap;

	private RDFSink addSink = new RDFSink()
	{
		private Sink<Statement, RippleException> stSink = new Sink<Statement, RippleException>()
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

		private Sink<Namespace, RippleException> nsSink = new Sink<Namespace, RippleException>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				contextToCollectorMap.get( null ).adderSink().namespaceSink().put( ns );
			}
		};

		private Sink<String, RippleException> comSink = new Sink<String, RippleException>()
		{
			public void put( final String comment ) throws RippleException
			{
				contextToCollectorMap.get( null ).adderSink().commentSink().put( comment );
			}
		};

		public Sink<Statement, RippleException> statementSink()
		{
			return stSink;
		}

		public Sink<Namespace, RippleException> namespaceSink()
		{
			return nsSink;
		}

		public Sink<String, RippleException> commentSink()
		{
			return comSink;
		}
	};

	private RDFSink subSink = new RDFSink()
	{
		private Sink<Statement, RippleException> stSink = new Sink<Statement, RippleException>()
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

		private Sink<Namespace, RippleException> nsSink = new Sink<Namespace, RippleException>()
		{
			public void put( final Namespace ns ) throws RippleException
			{
				contextToCollectorMap.get( null ).subtractorSink().namespaceSink().put( ns );
			}
		};

		private Sink<String, RippleException> comSink = new Sink<String, RippleException>()
		{
			public void put( final String comment ) throws RippleException
			{
				contextToCollectorMap.get( null ).subtractorSink().commentSink().put( comment );
			}
		};

		public Sink<Statement, RippleException> statementSink()
		{
			return stSink;
		}

		public Sink<Namespace, RippleException> namespaceSink()
		{
			return nsSink;
		}

		public Sink<String, RippleException> commentSink()
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

	public RdfDiffSource sourceForContext( final Resource context )
	{
		return contextToCollectorMap.get( context );
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

	public RDFSink adderSink()
	{
		return addSink;
	}

	public RDFSink subtractorSink()
	{
		return subSink;
	}
}

// kate: tab-width 4
