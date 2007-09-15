/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.cli;

import java.io.InputStream;

public class InputStreamEventFilter extends InputStream
{
	private InputStream source;
	private RecognizerAdapter recognizerAdapter;

	private int buffered;

	public InputStreamEventFilter( final InputStream is,
									final RecognizerAdapter rc )
	{
		source = is;
		recognizerAdapter = rc;

		buffered = -1;
	}

	public int read() throws java.io.IOException
	{
		if ( -1 != buffered )
		{
			int tmp = buffered;
			buffered = -1;
			return tmp;
		}

		// Break out when an ordinary character or sequence is found
		while( true )
		{
			int c = source.read();

			if ( 27 == c )
			{
				c = source.read();

				if ( 27 == c )
				{
					recognizerAdapter.putEvent( RecognizerEvent.ESCAPE );
				}

				else
				{
					buffered = c;
					return 27;
				}
			}

			else
			{
				return c;
			}
		}
	}

	public int available() throws java.io.IOException
	{
		// Note: ESC characters may throw this number off in either direction.
		return source.available();
	}
}

// kate: tab-width 4
