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

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;

public class ExportNsCmd extends Command
{
	String nsPrefix, fileName;

	public ExportNsCmd( final String nsPrefix, final String fileName )
	{
		this.nsPrefix = nsPrefix;
		this.fileName = fileName;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		OutputStream out;

		String ns = qe.getLexicon().resolveNamespacePrefix( nsPrefix );
		if ( null == ns )
			throw new RippleException( "namespace prefix '" + nsPrefix + "' is not defined" );

		try
		{
			out = new FileOutputStream( fileName );
		}

		catch ( java.io.FileNotFoundException e )
		{
			throw new RippleException( e );
		}

		mc.exportNs( ns, out );

		try
		{
			out.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		System.out.println( "\nExported namespace " + nsPrefix + " to " + fileName + "\n" );
	}

	protected void abort() {}
}

// kate: tab-width 4
