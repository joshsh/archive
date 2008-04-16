/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.media;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.StackContext;

import com.sun.speech.freetts.Voice;
import com.sun.speech.freetts.VoiceManager;

public class Speak extends PrimitiveStackMapping
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

	public void applyTo( final StackContext arg,
						 final Sink<StackContext, RippleException> sink )
		    throws RippleException
	{
        ModelConnection mc = arg.getModelConnection();
        RippleList stack = arg.getStack();

        String s = mc.toString( stack.getFirst() );
		//stack = stack.getRest();

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
		sink.put( arg.with( stack ) );
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
