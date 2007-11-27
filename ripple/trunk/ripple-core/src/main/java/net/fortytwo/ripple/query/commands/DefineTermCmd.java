/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import java.net.URL;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Collector;

import org.openrdf.model.URI;

	import net.fortytwo.ripple.rdf.diff.RdfDiff;
	import net.fortytwo.ripple.io.SparqlUpdater;
	import net.fortytwo.ripple.util.Sink;
	import org.openrdf.model.Statement;

public class DefineTermCmd extends Command
{
	private ListAst ast;
	private String term;

	public DefineTermCmd( final ListAst ast, final String term )
	{
		this.ast = ast;
		this.term = term;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		Collector<RippleValue> expressions = new Collector<RippleValue>();
		ast.evaluate( expressions, qe, mc );

		if ( expressions.size() == 0 )
		{
			qe.getErrorPrintStream().println(
				"Warning: the given expression did not resolve to a value." );
		}

		else if ( expressions.size() > 1 )
		{
			qe.getErrorPrintStream().println(
				"Warning: the given expression resolved to multiple values." );
		}

		else
		{
			RippleValue expr = expressions.iterator().next();

			if ( !( expr instanceof RippleList ) )
			{
				throw new RippleException( "term assignment for non-lists is not implemented" );
			}

			RippleList exprList = (RippleList) expr;
//System.out.println( "exprList = " + exprList );

// TODO: check for collision with an existing URI
			URI uri = mc.createUri( qe.getDefaultNamespace() + term );
			mc.copyStatements( exprList, new RdfValue( uri ) );

			qe.getLexicon().add( uri );
		}
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
