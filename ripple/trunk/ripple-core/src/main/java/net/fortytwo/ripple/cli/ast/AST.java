/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.flow.Sink;

public interface AST<T>
{
	void evaluate( Sink<T, RippleException> sink,
					QueryEngine qe,
					ModelConnection mc )
		throws RippleException;
}

// kate: tab-width 4
