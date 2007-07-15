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
	public Speak()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		if ( !initialized )
			init();

		String s;

		s = mc.stringValue( stack.getFirst() );
		stack = stack.getRest();

		singleVoice.speak( s );
	}

	static Voice singleVoice;

	static boolean initialized = false;

	static void init()
		throws RippleException
	{
		String voiceName = "kevin";

		VoiceManager voiceManager = VoiceManager.getInstance();
		singleVoice = voiceManager.getVoice( voiceName );

		if ( null == singleVoice )
			throw new RippleException(
				"Cannot find a voice named " + voiceName );
		
		singleVoice.allocate();

		initialized = true;
	}

	// Note: never called.
	static void shutdown()
	{
		if ( initialized )
			singleVoice.deallocate();
	}
}

// kate: tab-width 4
