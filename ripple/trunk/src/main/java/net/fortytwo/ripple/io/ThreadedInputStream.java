/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.control.Scheduler;
import net.fortytwo.ripple.control.Task;

/**
 * An InputStream which optionally spawns a new thread to read bytes from a
 * source InputStream into a buffer as soon as they are available.  Otherwise,
 * it acts as a simple wrapper for the source InputStream, reading from the
 * source only when its own read() method is called.
 */
public class ThreadedInputStream extends InputStream
{
	InputStream source;
	PipedInputStream writeIn;
	PipedOutputStream readOut;
	boolean eager;
	Task readerTask;

	public ThreadedInputStream( final InputStream is )
		throws RippleException
	{
		source = is;

		eager = false;
		readerTask = null;

		try
		{
			writeIn = new PipedInputStream();
			readOut = new PipedOutputStream( writeIn );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	public void setEager( final boolean eager )
	{
//System.out.println( "setEager( " + eager + " )" );
		if ( eager )
		{
			if ( !this.eager )
			{
//System.out.println( "    enabling reader task" );
				this.eager = true;

				if ( null == readerTask )
				{
					createTask();
				}

				Scheduler.add( readerTask );
			}
		}

		else
		{
			if ( this.eager )
			{
//System.out.println( "    disabling reader task" );
				this.eager = false;
				readerTask.stop();
			}
		}
//System.out.println( "    done." );
	}

	public int available() throws java.io.IOException
	{
		synchronized ( writeIn )
		{
			return writeIn.available();
		}
	}

	public int read() throws java.io.IOException
	{
//System.out.println( "read()" );
		if ( 0 == available() )
		{
			requestByte( true );
		}

		synchronized ( writeIn )
		{
			return writeIn.read();
		}
	}

	synchronized void requestByte( final boolean forRead )
		throws java.io.IOException
	{
//System.out.println( "requestByte( " + forRead + " )" );
//System.out.println( "    writeIn.available() = " + writeIn.available() );
		if ( ( !forRead || !( writeIn.available() > 0 ) )
				&& ( forRead || eager ) )
		{
//System.out.println( "    reading..." );
			// This will sometimes block.
			int c = source.read();
//System.out.println( "    done reading" );

			synchronized ( writeIn )
			{
				readOut.write( c );
			}
		}
	}

	void createTask()
	{
		readerTask = new Task()
		{
			boolean active = false;

			protected void executeProtected() throws RippleException
			{
				active = true;

				while ( active )
				{
					try
					{
						requestByte( false );
					}

					catch ( java.io.IOException e )
					{
						throw new RippleException( e );
					}
				}
			}

			protected void stopProtected()
			{
				active = false;
			}
		};
	}
}

// kate: tab-width 4
