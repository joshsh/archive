/*
 * $URL: $
 * $Revision: $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import java.util.Properties;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.flow.Sink;

public class PropertyAnnotatedAst implements Ast<RippleList>
{
	private Ast innerAst;
	private Properties props;
	
	public PropertyAnnotatedAst( final Ast inner, final Properties props )
	{
		innerAst = inner;
		this.props = props;
	}
	
	public void evaluate( Sink<RippleList, RippleException> sink,
					QueryEngine qe,
					ModelConnection mc )
		throws RippleException
	{
// TODO: create a PropertyAnnotatedValue class and translate into it
		innerAst.evaluate( sink, qe, mc );
	}
}

// kate: tab-width 4

