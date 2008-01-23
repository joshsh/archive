package net.fortytwo.ripple.query;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class PipedIOStream extends InputStream //, OutputStream
{
	private static final int BUFFER_EXPANSION = 2;
	private static final int DEFAULT_INITIAL_SIZE = 200;
	
	private String mutex = "";
	private int pos, length, size;
	private int[] data;
	
	public PipedIOStream()
	{
		size = DEFAULT_INITIAL_SIZE;
		data = new int[size];
		pos = 0;
		length = 0;
	}

	@Override
	public synchronized void close() throws IOException
	{
		data = null;
		
		synchronized ( mutex )
		{
			mutex.notify();
		}
	}

	@Override
	public synchronized int available()
	{
		return length;	
	}

	@Override
	public int read() throws IOException
	{
		if ( 0 == length )
		{
			synchronized ( mutex )
			{
				try {
					mutex.wait();
				} catch (InterruptedException e) {
					throw new IOException( e.toString() );
				}
			}
		}
		
		synchronized ( this )
		{
			if ( null == data )
			{
				throw new IOException( "can't read: pipe has been closed: " + this );
			}
			
			int c = data[pos];
			pos = ( 1 + pos ) % size;
			length--;
			return c;
		}
	}

	public synchronized void write( int b ) throws IOException
	{
		if ( null == data )
		{
			throw new IOException( "can't write: pipe has been closed" );
		}

		// Expand the buffer if needed.
		if ( length + 1 > size )
		{
			int newSize = size * BUFFER_EXPANSION;
			int[] newData = new int[newSize];
			for ( int i = 0; i < length; i++ )
			{
				newData[i] = data[( pos + i ) % size];
			}

			pos = 0;
			data = newData;
			size = newSize;
		}

		data[( pos + length ) % size] = b;
		length++;

		synchronized ( mutex )
		{
			mutex.notify();
		}
	}

	public void write( final byte[] b ) throws IOException
	{
		for ( int i = 0; i < b.length; i++ )
		{
			write( b[i] );
		}
	}
}
