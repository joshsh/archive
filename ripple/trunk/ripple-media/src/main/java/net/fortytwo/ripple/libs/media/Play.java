/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.media;

import java.io.InputStream;
import java.io.IOException;

import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequencer;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.StackContext;
import net.fortytwo.ripple.util.HTTPUtils;

import org.openrdf.model.URI;
import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.HttpClient;

public class Play extends PrimitiveStackMapping
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

    public void applyTo( final StackContext arg,
                         final Sink<StackContext, RippleException> sink )
            throws RippleException
	{
        ModelConnection mc = arg.getModelConnection();
        RippleList stack = arg.getStack();

        URI uri = mc.createUri( mc.toString( stack.getFirst() ) );
		//stack = stack.getRest();

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
		sink.put( arg.with( stack ) );
	}

	private void play( final URI uri ) throws RippleException
	{
		String[] mimeTypes = { "audio/midi" };
        HttpMethod method = HTTPUtils.createRdfGetMethod( uri.toString() );
        HTTPUtils.setAcceptHeader( method, mimeTypes );
        HTTPUtils.registerMethod( method );
		HttpClient client = HTTPUtils.createClient();

        InputStream is;

        try {
            client.executeMethod( method );
            is = method.getResponseBodyAsStream();
        } catch ( IOException e ) {
            throw new RippleException( e );
        }

        try {
            play( is );
        } finally {
            try {
                is.close();
            } catch (IOException e) {
                throw new RippleException( e );
            }
        }
	}

	// Note: we won't try to play more than one MIDI at a time.
	private synchronized void play( final InputStream is )
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
