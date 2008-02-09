/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.graph;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.PrimitiveStackRelation;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Statement;

/**
 * A primitive which consumes a resource and produces a three-element list
 * (subject, resource, object) for each statement about the resource.
 */
public class Links extends PrimitiveStackRelation
{
	private static final int ARITY = 1;

	public Links()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( final StackContext arg,
						 final Sink<StackContext> sink
	)
		throws RippleException
	{
		final ModelConnection mc = arg.getModelConnection();
		RippleList stack = arg.getStack();

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

				RippleList triple = mc.list( obj ).push( pred ).push( subj );

				sink.put( arg.with(
					rest.push( triple ) ) );
			}
		};

		mc.getStatements( subj.toRdf( mc ), null, null, stSink, Ripple.useInference() );
	}
}

// kate: tab-width 4
