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
import net.fortytwo.ripple.cli.ast.UriAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Collector;

import org.openrdf.model.URI;

public class DefineTermCmd extends Command
{
	private ListAst list;
	private String name;

	public DefineTermCmd( final String name, final ListAst list)
	{
		this.list = list;
		this.name = name;
	}
        
    public String getName()
    {
        return name;
    }

    public ListAst getList()
    {
        return list;
    }

    public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		Collector<RippleList> expressions = new Collector<RippleList>();
		list.evaluate( expressions, qe, mc );

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
			// Note: the first element of the list will also be a list
			RippleList expr = (RippleList) expressions.iterator().next().getFirst();

//System.out.println( "exprList = " + exprList );

// TODO: check for collision with an existing URI
			URI uri = mc.createUri( qe.getDefaultNamespace() + name);
			mc.copyStatements( expr, new RdfValue( uri ) );
			mc.commit();

			qe.getLexicon().add( uri );
		}
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
