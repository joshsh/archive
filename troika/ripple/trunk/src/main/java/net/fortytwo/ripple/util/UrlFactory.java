package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;

import java.net.MalformedURLException;
import java.net.URL;

import java.util.Arrays;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class UrlFactory
{
	private final static Logger s_logger
		= Logger.getLogger( UrlFactory.class );

	private Map<String, String> map;

	private String [] sourceUrls;
	private String [] targetUrls;

	private boolean upToDate = true;

	public UrlFactory()
	{
		map = new Hashtable<String, String>();
	}

	private URL createUrlPrivate( final String urlStr )
		throws RippleException
	{
		try
		{
			return new URL( urlStr );
		}

		catch ( MalformedURLException e )
		{
			throw new RippleException( e );
		}
	}

	private void update()
	{
		Set<String> keySet = map.keySet();

		sourceUrls = new String[ keySet.size() ];
		Iterator<String> keySetIter = keySet.iterator();
		int j = 0;
		while ( keySetIter.hasNext() )
			sourceUrls[j++] = keySetIter.next();
		Arrays.sort( sourceUrls );

		targetUrls = new String[ sourceUrls.length ];
		for ( int i = 0; i < sourceUrls.length; i++ )
		{
			targetUrls[i] = map.get( sourceUrls[i] );
			s_logger.debug( "map " + sourceUrls[i] + " to " + targetUrls[i] );
		}

		upToDate = true;
	}

	public void addMapping( final String source, final String dest )
	{
		upToDate = false;

		map.put( source, dest );
	}

	public URL createUrl( final String urlStr )
		throws RippleException
	{
		if ( !upToDate )
			update();

		int fromIndex = 0, toIndex = sourceUrls.length - 1;
		int mid = 0, cmp = 0;
		int i = -1;

		while ( fromIndex <= toIndex )
		{
			mid = ( fromIndex + toIndex ) / 2;
			cmp = urlStr.compareTo( sourceUrls[mid] );

			if ( cmp > 0 )
			{
				fromIndex = mid + 1;
				i = mid;
			}

			else if ( cmp < 0 )
				toIndex = mid - 1;

			// Exact match.  Return the corresponding target URL.
			else
				return createUrlPrivate( targetUrls[mid] );
		}

		if ( -1 < i && urlStr.startsWith( sourceUrls[i] ) )
			return createUrlPrivate(
				targetUrls[i] + urlStr.substring( sourceUrls[i].length() ) );

		// No rewrite necessary.
		else
			return createUrlPrivate( urlStr );
	}
}

// kate: tab-width 4
