/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.util.Sink;

public class OperatorAst implements Ast
{
	public enum Type { Apply, Option, Star, Plus, Times, Range };

	private Type type;
	private int min, max;

	public OperatorAst()
	{
		this( Type.Apply );
	}

	public OperatorAst( final Type type )
	{
		this.type = type;
	}

	public OperatorAst( final int times )
	{
		type = Type.Times;
		min = times;
	}

	public OperatorAst( final int min, final int max )
	{
		type = Type.Range;
		this.min = min;
		this.max = max;
	}

	public void evaluate( final Sink<RippleValue> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		switch ( type )
		{
			case Apply:
				sink.put( Operator.OP );
				break;
			case Option:
				sink.put( new Operator( StackLibrary.getIoptValue() ) );
				break;
			case Star:
				sink.put( new Operator( StackLibrary.getIstarValue() ) );
				break;
			case Plus:
				sink.put( new Operator( StackLibrary.getIplusValue() ) );
				break;
			/* TODO
			case Times:
			case Range:*/
		}
	}

	public String toString()
	{
		switch ( type )
		{
			case Apply:
				return "!";
			case Option:
				return "?";
			case Star:
				return "*";
			case Plus:
				return "+";
			case Times:
				return "{" + min + "}";
			case Range:
				return "{" + min + "," + max + "}";
			default:
				return "error";
		}
	}
}

// kate: tab-width 4
