package net.fortytwo.ripple.query;

import java.io.IOException;
import java.io.InputStream;

public class PipedIOStream extends InputStream
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
				throw new IOException( "pipe has been closed" );
			}
			
			int c = data[pos];
			pos = ( 1 + pos ) % size;
			length--;
			return c;
		}
	}

	synchronized void write( final byte[] b ) throws IOException
	{
		// Expand the buffer if needed.
		if ( length + b.length > size )
		{
			size *= BUFFER_EXPANSION;
			int[] newData = new int[size];
			for ( int i = 0; i < length; i++ )
			{
				newData[i] = data[pos + i];
			}
			
			pos = 0;
			data = newData;
		}
		
		for ( int i = 0; i < b.length; i++ )
		{
			data[( i + pos ) % size] = b[i];
		}
		
		length += b.length;
		
		synchronized ( mutex )
		{
			mutex.notify();
		}
	}
}
