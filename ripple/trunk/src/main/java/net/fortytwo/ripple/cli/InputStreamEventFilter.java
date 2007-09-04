package net.fortytwo.ripple.cli;

import java.io.InputStream;

public class InputStreamEventFilter extends InputStream
{
	InputStream source;
	RecognizerCoupling recognizerCoupling;

	int buffered;

	public InputStreamEventFilter( final InputStream is,
									final RecognizerCoupling rc )
	{
		source = is;
		recognizerCoupling = rc;

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
					recognizerCoupling.putEvent( RecognizerEvent.ESCAPE );
				else
				{
					buffered = c;
					return 27;
				}
			}

			else
				return c;
		}
	}
}

// kate: tab-width 4
