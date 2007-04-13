package net.fortytwo.ripple.cli.commands;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;

import org.openrdf.model.URI;

public class DefineTermCmd implements Command
{
	private ListAst ast;
	private String term;

	public DefineTermCmd( ListAst ast, final String term )
	{
		this.ast = ast;
		this.term = term;
	}

	public void execute( QueryContext qc, ModelConnection mc )
		throws RippleException
	{
		ContainerSink expressions = new ContainerSink();
		ast.evaluate( expressions, qc, mc );

		if ( expressions.size() == 0 )
			qc.getErrorPrintStream().println(
				"Error: the given expression did not resolve to a value." );

		else if ( expressions.size() > 1 )
			qc.getErrorPrintStream().println(
				"Error: the given expression resolved to multiple values." );

		else
		{
			RippleValue expr = expressions.iterator().next();

			if ( !( expr instanceof RippleList ) )
				throw new RippleException( "term assignment for non-lists is not implemented" );

			RippleList exprList = (RippleList) expr;

// TODO: check for collision with an existing URI
			URI uri = mc.createUri( qc.getDefaultNamespace() + term );

			mc.copyStatements( exprList.toRdf( mc ), new RdfValue( uri ) );

			qc.getLexicon().update();
		}
	}
}

// kate: tab-width 4
