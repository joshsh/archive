package net.fortytwo.ripple.cli;

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
	boolean eager = false;
	Task readerTask = null;

	public ThreadedInputStream( final InputStream is )
		throws RippleException
	{
		source = is;

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
//System.out.println( "setEager()" );
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

	public int read() throws java.io.IOException
	{
		synchronized ( source )
		{
			if ( 0 == writeIn.available() )
			{
				readOut.write( source.read() );
			}
		}

		return writeIn.read();
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
						synchronized ( source )
						{
							readOut.write( source.read() );
						}
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

	public int available()
	{
// TODO
return 0;
	}
}

// kate: tab-width 4
