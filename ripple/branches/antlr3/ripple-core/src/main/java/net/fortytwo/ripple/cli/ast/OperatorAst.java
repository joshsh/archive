/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.libs.stack.StackLibrary;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.QueryEngine;

public class OperatorAst implements Ast<RippleList>
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

	public void evaluate( final Sink<RippleList, RippleException> sink,
						final QueryEngine qe,
						final ModelConnection mc )
		throws RippleException
	{
		RippleList l;

		switch ( type )
		{
			case Apply:
				l = mc.list( Operator.OP );
				break;
			case Option:
				l = mc.list( Operator.OP ).push( StackLibrary.getOptApplyValue() );
				break;
			case Star:
				l = mc.list( Operator.OP ).push( StackLibrary.getStarApplyValue() );
				break;
			case Plus:
				l = mc.list( Operator.OP ).push( StackLibrary.getPlusApplyValue() );
				break;
			case Times:
				l = mc.list( Operator.OP )
						.push( StackLibrary.getTimesApplyValue() )
						.push( mc.value( min ) );
				break;
			case Range:
				l = mc.list( Operator.OP )
						.push( StackLibrary.getRangeApplyValue() )
						.push( mc.value( max ) )
						.push( mc.value( min ) );
				break;
			default:
				throw new RippleException( "unhandled operator type: " + type );	
		}

		sink.put( l );
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
