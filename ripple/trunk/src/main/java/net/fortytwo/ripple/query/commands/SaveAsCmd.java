/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import java.io.FileOutputStream;
import java.io.OutputStream;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.io.CacheManager;

public class SaveAsCmd extends Command
{
	private String fileName;

	public SaveAsCmd( final String fileName )
	{
		this.fileName = fileName;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		OutputStream out;

		try
		{
			out = new FileOutputStream( fileName );
		}

		catch ( java.io.FileNotFoundException e )
		{
			throw new RippleException( e );
		}

		CacheManager.writeCacheTo( out, Ripple.cacheFormat(), mc );

		try
		{
			out.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		System.out.println( "\nSaved data set as " + fileName + "\n" );
	}

	protected void abort()
	{
	}
}

// kate: tab-width 4
