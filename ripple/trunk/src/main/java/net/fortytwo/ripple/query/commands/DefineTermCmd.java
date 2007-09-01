/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

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

	import net.fortytwo.ripple.io.RdfDiff;
	import net.fortytwo.ripple.io.SparqlUpdater;
	import net.fortytwo.ripple.util.Sink;
	import org.openrdf.model.Statement;

public class DefineTermCmd extends Command
{
	ListAst ast;
	String term;

	public DefineTermCmd( ListAst ast, final String term )
	{
		this.ast = ast;
		this.term = term;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
		throws RippleException
	{
		Collector<RippleValue> expressions = new Collector<RippleValue>();
		ast.evaluate( expressions, qe, mc );

		if ( expressions.size() == 0 )
			qe.getErrorPrintStream().println(
				"Error: the given expression did not resolve to a value." );

		else if ( expressions.size() > 1 )
			qe.getErrorPrintStream().println(
				"Error: the given expression resolved to multiple values." );

		else
		{
			RippleValue expr = expressions.iterator().next();

			if ( !( expr instanceof RippleList ) )
				throw new RippleException( "term assignment for non-lists is not implemented" );

			RippleList exprList = (RippleList) expr;

// TODO: check for collision with an existing URI
			URI uri = mc.createUri( qe.getDefaultNamespace() + term );

			mc.copyStatements( exprList.toRdf( mc ), new RdfValue( uri ) );

			qe.getLexicon().add( uri );

pushListToSemWeb( exprList, mc );
		}
	}

	protected void abort() {}





void pushListToSemWeb( final RippleList list, final ModelConnection mc ) throws RippleException
{
	final RdfDiff diff = new RdfDiff();

	Sink<Statement> diffAdderSink = new Sink<Statement>()
	{
		public void put( final Statement st ) throws RippleException
		{
			diff.add( st );
		}
	};

	list.writeStatementsTo( diffAdderSink, mc );

	SparqlUpdater.postUpdate( diff, null );
}





}

// kate: tab-width 4
