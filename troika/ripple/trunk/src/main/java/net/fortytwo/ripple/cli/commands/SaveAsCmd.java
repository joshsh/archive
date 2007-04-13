package net.fortytwo.ripple.cli.commands;

import java.io.FileOutputStream;
import java.io.OutputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Command;
import net.fortytwo.ripple.cli.QueryContext;
import net.fortytwo.ripple.model.ModelConnection;

public class SaveAsCmd implements Command
{
	private String fileName;

	public SaveAsCmd( final String fileName )
	{
		this.fileName = fileName;
	}

	public void execute( QueryContext qc, ModelConnection mc )
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

		qc.getModel().writeTo( out );
//        qc.getModel().writeTrix( out );

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
