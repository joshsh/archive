package net.fortytwo.ripple.rdf.sail;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.diff.RdfDiffSink;
import net.fortytwo.ripple.rdf.RdfSink;
import net.fortytwo.ripple.util.NullSink;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

// Note: for maximum clarity, this class should probably be called
//       "LinkedDataSailConnectionOutputAdapter"
public class SailConnectionOutputAdapter implements RdfDiffSink
{
	private LinkedDataSailConnection sailConnection;
	private RdfSink addSink, subtractSink;

	public SailConnectionOutputAdapter( final LinkedDataSailConnection sc )
	{
		sailConnection = sc;

		addSink = new RdfSink()
		{
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
//System.out.println( "adding statement: " + st );
					sailConnection.addStatement( st );
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
//System.out.println( "adding namespace: " + ns );
					sailConnection.addNamespace( ns );
				}
			};

			private Sink<String> cmtSink = new NullSink<String>();

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

		subtractSink = new RdfSink()
		{
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
					sailConnection.removeStatement( st );
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
					sailConnection.removeNamespace( ns );
				}
			};

			private Sink<String> cmtSink = new NullSink<String>();

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
		return subtractSink;
	}
}

// kate: tab-width 4
