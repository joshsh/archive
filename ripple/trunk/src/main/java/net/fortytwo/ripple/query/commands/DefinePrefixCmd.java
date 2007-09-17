/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.UriAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Collector;

import org.openrdf.model.URI;
import org.openrdf.model.impl.NamespaceImpl;

public class DefinePrefixCmd extends Command
{
	private String prefix;
	private UriAst uri;

	public DefinePrefixCmd( final String prefix, final UriAst uri )
	{
		this.prefix = prefix;
		this.uri = uri;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		Collector<RippleValue> sink = new Collector<RippleValue>();
		uri.evaluate( sink, qe, mc );

		if ( sink.size() == 0 )
		{
			throw new RippleException( "URI could not be constructed from " + uri );
		}

		else if ( sink.size() > 1 )
		{
			throw new RippleException( "multiple values constructed from " + uri );
		}

		URI ns = mc.uriValue( sink.iterator().next() );
		mc.setNamespace( prefix, ns, true );

		// Note: when a namespace is manually defined, it may both override an
		// existing prefix with the same name, or duplicate another namespace
		// with the same URI.
		qe.getLexicon().add( new NamespaceImpl( prefix, ns.toString() ) );
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
