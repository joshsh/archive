package net.fortytwo.ripple.rdf;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.util.Sink;
import org.openrdf.model.BNode;
import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.ValueFactory;

/**
 * Author: josh
 * Date: Jan 18, 2008
 * Time: 12:52:56 PM
 */
public class BNodeToUriFilter implements RdfSink
{
	private Sink<Statement> stSink;
	private Sink<Namespace> nsSink;
	private Sink<String> cmtSink;

	private ValueFactory valueFactory;

	public BNodeToUriFilter( final RdfSink sink, final ValueFactory vf )
	{
		valueFactory = vf;

		final Sink<Statement> destStSink = sink.statementSink();

		stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				boolean s = st.getSubject() instanceof BNode;
				boolean o = st.getObject() instanceof BNode;
				boolean c = ( null == st.getContext() ) ? false : st.getContext() instanceof BNode;

				if ( s || o || c )
				{
					Resource subj = s ? bnodeToUri( (BNode) st.getSubject() ) : st.getSubject();
					URI pred = st.getPredicate();
					Value obj = o ? bnodeToUri( (BNode) st.getObject() ) : st.getObject();
					Resource con = c ? bnodeToUri( (BNode) st.getContext() ) : st.getContext();

					Statement newSt = ( null == con )
							? valueFactory.createStatement( subj, pred, obj )
							: valueFactory.createStatement( subj, pred, obj, con );

					destStSink.put( newSt );
				}

				else
				{
					destStSink.put( st );
				}
			}
		};

		nsSink = sink.namespaceSink();
		cmtSink = sink.commentSink();
	}

	private URI bnodeToUri( final BNode bnode )
	{
		return valueFactory.createURI( Ripple.URN_BNODE_PREFIX + bnode.getID() );
	}
	
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
}
