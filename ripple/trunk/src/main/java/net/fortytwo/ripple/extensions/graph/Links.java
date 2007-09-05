/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.graph;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Statement;

public class Links extends PrimitiveFunction
{
	public Links()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		RippleValue subj;

		subj = stack.getFirst();
		final RippleList rest = stack.getRest();

		final ModelBridge bridge = mc.getModel().getBridge();

		Sink<Statement> stSink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				RippleValue subj = bridge.get( st.getSubject() );
				RippleValue pred = bridge.get( st.getPredicate() );
				RippleValue obj = bridge.get( st.getObject() );

				sink.put(
					new RippleList( subj, rest ).push( pred ).push( obj ) );
			}
		};

		mc.getStatements( subj.toRdf( mc ), null, null, stSink );
	}
}

// kate: tab-width 4
