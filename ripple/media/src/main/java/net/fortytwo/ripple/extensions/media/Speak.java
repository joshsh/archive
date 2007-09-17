/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.media;

import com.sun.speech.freetts.Voice;
import com.sun.speech.freetts.VoiceManager;
import com.sun.speech.freetts.audio.JavaClipAudioPlayer;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class Speak extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private Voice singleVoice = null;

	public Speak()
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
		String s;

		s = mc.stringValue( stack.getFirst() );
//		stack = stack.getRest();

		try
		{
			speak( s );
		}

		catch ( RippleException e )
		{
System.out.println( "error: " + e );
			e.logError();
		}

		// Pass the stack along, unaltered.
		sink.put( stack );
	}

	// Note: we won't try to speak more than one expression at a time.
	synchronized void speak( final String s )
		throws RippleException
	{
		if ( null == singleVoice )
		{
			createVoice();
		}

		singleVoice.speak( s );
	}

	private void createVoice()
		throws RippleException
	{
		String voiceName = "kevin";

		try
		{
			VoiceManager voiceManager = VoiceManager.getInstance();
			singleVoice = voiceManager.getVoice( voiceName );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		if ( null == singleVoice )
		{
			throw new RippleException(
				"Cannot find a voice named " + voiceName );
		}

		try
		{
			singleVoice.allocate();
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}

	// Note: never called.
	private synchronized void end() throws RippleException
	{
		try
		{
			if ( null != singleVoice )
			{
				singleVoice.deallocate();
			}
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}
	}
}

// kate: tab-width 4
