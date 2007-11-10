package net.fortytwo.ripple.server;

import java.util.Iterator;

import org.restlet.Component;
import org.restlet.Restlet;
import org.restlet.Server;
import org.restlet.data.CharacterSet;
import org.restlet.data.ClientInfo;
import org.restlet.data.Encoding;
import org.restlet.data.Language;
import org.restlet.data.MediaType;
import org.restlet.data.Preference;
import org.restlet.data.Protocol;
import org.restlet.data.Reference;
import org.restlet.data.Request;
import org.restlet.data.Response;

public class InfoRestlet extends Restlet
{
	@Override
	public void handle( final Request request, final Response response )
	{
		StringBuilder message = new StringBuilder();

		Reference resourceRef = request.getResourceRef();
		message.append( "Resource URI  : " ).append( resourceRef.toString() ).append( '\n' );

		Reference rootRef = request.getRootRef();
		message.append( "Root URI      : " ).append( rootRef.toString() ).append( '\n' );

		Reference baseRef = resourceRef.getBaseRef();
		message.append( "Routed part   : " ).append( baseRef.toString() ).append( '\n' );

		String remainingPart = resourceRef.getRemainingPart();
		message.append( "Remaining part: " ).append( remainingPart ).append( '\n' );

		message.append( '\n' );

		ClientInfo clientInfo = request.getClientInfo();

		Iterator<Preference<CharacterSet>> acceptedCharacterSets = clientInfo.getAcceptedCharacterSets().iterator();
		message.append( "Accepted character sets:\n" );
		while ( acceptedCharacterSets.hasNext() )
		{
			Preference<CharacterSet> set = acceptedCharacterSets.next();
			message.append( '\t' ).append( set.toString() ).append( '\n' );
		}

		Iterator<Preference<Encoding>> acceptedEncodings = clientInfo.getAcceptedEncodings().iterator();
		message.append( "Accepted encodings:\n" );
		while ( acceptedEncodings.hasNext() )
		{
			Preference<Encoding> encoding = acceptedEncodings.next();
			message.append( '\t' ).append( encoding.toString() ).append( '\n' );
		}

		Iterator<Preference<Language>> acceptedLanguages = clientInfo.getAcceptedLanguages().iterator();
		message.append( "Accepted languages:\n" );
		while ( acceptedLanguages.hasNext() )
		{
			Preference<Language> lang = acceptedLanguages.next();
			message.append( '\t' ).append( lang.toString() ).append( '\n' );
		}

		Iterator<Preference<MediaType>> acceptedMediaTypes = clientInfo.getAcceptedMediaTypes().iterator();
		message.append( "Accepted media types:\n" );
		while ( acceptedMediaTypes.hasNext() )
		{
			Preference<MediaType> mediaTypePreference = acceptedMediaTypes.next();
			message.append( '\t' ).append( mediaTypePreference.toString() ).append( '\n' );
		}

		Iterator<String> addresses = clientInfo.getAddresses().iterator();
		message.append( "Addresses:\n" );
		while ( addresses.hasNext() )
		{
			String address = addresses.next();
			message.append( '\t' ).append( address ).append( '\n' );
		}

		String agent = clientInfo.getAgent();
		message.append( "Agent name: " ).append( agent ).append( '\n' );

		int port = clientInfo.getPort();
		message.append( "Port: " ).append( port ).append( '\n' );

		response.setEntity( message.toString(), MediaType.TEXT_PLAIN );
	}
}

// kate: tab-width 4
