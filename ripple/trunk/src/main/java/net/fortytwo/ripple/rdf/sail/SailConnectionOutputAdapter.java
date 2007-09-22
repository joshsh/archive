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

public class SailConnectionOutputAdapter implements RdfDiffSink
{
	private SailConnection sailConnection;
	private RdfSink addSink, subtractSink;

	public SailConnectionOutputAdapter( final SailConnection sc )
	{
		sailConnection = sc;

		addSink = new RdfSink()
		{
			private Sink<Statement> stSink = new Sink<Statement>()
			{
				public void put( final Statement st ) throws RippleException
				{
					Resource context = st.getContext();

					try
					{
						if ( null == context )
						{
							sailConnection.addStatement(
								st.getSubject(),
								st.getPredicate(),
								st.getObject() );
						}

						else
						{
							sailConnection.addStatement(
								st.getSubject(),
								st.getPredicate(),
								st.getObject(),
								context );
						}
					}
	
					catch ( SailException e )
					{
						throw new RippleException( e );
					}
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
					try
					{
						sailConnection.setNamespace( ns.getPrefix(), ns.getName() );
					}
	
					catch ( SailException e )
					{
						throw new RippleException( e );
					}
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
					Resource context = st.getContext();

					try
					{
						if ( null == context )
						{
							sailConnection.removeStatements(
								st.getSubject(),
								st.getPredicate(),
								st.getObject() );
						}

						else
						{
							sailConnection.removeStatements(
								st.getSubject(),
								st.getPredicate(),
								st.getObject(),
								context );
						}
					}
	
					catch ( SailException e )
					{
						throw new RippleException( e );
					}
				}
			};

			private Sink<Namespace> nsSink = new Sink<Namespace>()
			{
				public void put( final Namespace ns ) throws RippleException
				{
					try
					{
						// Note: removes the namespace with the given prefix,
						// regardless of the associated URI.
						sailConnection.removeNamespace( ns.getPrefix() );
					}
	
					catch ( SailException e )
					{
						throw new RippleException( e );
					}
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
