package net.fortytwo.ripple.query.commands;

import java.io.FileOutputStream;
import java.io.OutputStream;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.io.CacheManager;

public class SaveAsCmd implements Command
{
	private String fileName;

	public SaveAsCmd( final String fileName )
	{
		this.fileName = fileName;
	}

	public void execute( QueryEngine qe, ModelConnection mc )
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

		CacheManager.writeCacheTo( qe.getModel(), out, Ripple.cacheFormat() );

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
}

// kate: tab-width 4
