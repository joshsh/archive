/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.media;

import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequencer;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

public class Play extends PrimitiveFunction
{
	private static final int ARITY = 1;

	public Play()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
//		stack = stack.getRest();

		try
		{
			play( uri );
		}

		catch ( RippleException e )
		{
System.out.println( "error: " + e );
			e.logError();
		}

		// Pass the stack along, unaltered.
		sink.put( stack );
	}

	void play( final URI uri )
		throws RippleException
	{
		URLConnection conn;

		try
		{
			URL url = new URL( uri.toString() );
			conn = url.openConnection();
		}

		catch ( java.net.MalformedURLException e )
		{
			throw new RippleException( e );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		String []mimeTypes = { "audio/midi" };
		HttpUtils.prepareUrlConnectionForRequest( conn, mimeTypes );

		InputStream is;

		try
		{
			conn.connect();
			is = conn.getInputStream();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		play( is );

		try
		{
			is.close();
		}

		catch( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	// Note: we won't try to play more than one MIDI at a time.
	synchronized void play( final InputStream is )
		throws RippleException
	{
		// Play once
		try
		{
			Sequencer sequencer = MidiSystem.getSequencer();
			sequencer.setSequence( MidiSystem.getSequence( is ) );
			sequencer.open();
			sequencer.start();

			// Break out when playback is finished.
			while( true )
			{
				if( sequencer.isRunning() )
				{
					try
					{
						Thread.sleep( 1000 ); // Check every second
					}

					catch( InterruptedException ignore)
					{
						break;
					}
				}

				else
					break;
			}

			// Close the MidiDevice & free resources
			sequencer.stop();
			sequencer.close();

			is.close();
		}

		catch( javax.sound.midi.MidiUnavailableException e )
		{
			throw new RippleException( e );
		}

		catch( javax.sound.midi.InvalidMidiDataException e )
		{
			throw new RippleException( e );
		}

		catch( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
