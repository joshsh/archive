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

	import net.fortytwo.ripple.io.RdfDiff;
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

			mc.copyStatements( exprList.toRdf( mc ), new RdfValue( uri ) );

			qe.getLexicon().add( uri );

pushListToSemWeb( uri, mc );
		}
	}

	protected void abort()
	{
	}





private void pushListToSemWeb( final URI uri,
								final ModelConnection mc )
	throws RippleException
{
	final RdfDiff diff = new RdfDiff();

	final Sink<Statement> diffAdderSink = new Sink<Statement>()
	{
		public void put( final Statement st ) throws RippleException
		{
//System.out.println( "adding statement: " + st );
			diff.add( st );
		}
	};

// FIXME: reading the list from the RDF model is backwards
	Sink<RippleList> listSink = new Sink<RippleList>()
	{
		public void put( final RippleList list ) throws RippleException
		{
//System.out.println( "list = " + list );
			list.writeStatementsTo( diffAdderSink, mc );
		}
	};

	RippleList.from( new RdfValue( uri ), listSink, mc );

	URL url;

	try
	{
		String s = uri.toString();
		int hashIndex = s.lastIndexOf( '#' );
		if ( hashIndex >= 0 )
		{
			s = s.substring( 0, hashIndex );
		}

		url = new URL( s );
System.out.println( "url =" + url );
	}

	catch ( java.net.MalformedURLException e )
	{
		throw new RippleException( e );
	}

	SparqlUpdater.postUpdate( diff, url );
}





}

// kate: tab-width 4
