package net.fortytwo.ripple.util;

import java.io.InputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import java.util.Collection;
import java.util.HashSet;
import java.util.Iterator;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;

public class ExtensionLoader extends ClassLoader
{
	public ExtensionLoader()
	{
		super( ExtensionLoader.class.getClassLoader() );
	}

	public void load( final UrlFactory uf, final ModelConnection mc )
		throws RippleException
	{
		for ( Iterator<String> namesIter = getNames().iterator();
			namesIter.hasNext(); )
		{
			String className = namesIter.next();

			Class c;
			Extension ext;

			try
			{
				c = loadClass( className );
			}

			catch ( ClassNotFoundException e )
			{
				throw new RippleException( e );
			}

			try
			{
				ext = (Extension) c.newInstance();
			}

			catch ( InstantiationException e )
			{
				throw new RippleException( e );
			}

			catch ( IllegalAccessException e )
			{
				throw new RippleException( e );
			}

			ext.load( uf, mc );
		}
	}

	Collection<String> getNames()
		throws RippleException
	{
		HashSet<String> names = new HashSet<String>();

		try
		{
			InputStream is
				= net.fortytwo.ripple.Ripple.class.getResourceAsStream(
					"extensions.txt" );

			BufferedReader reader = new BufferedReader(
				new InputStreamReader( is ) );
	
			// Break out when end of stream is reached.
			while ( true )
			{
				String line = reader.readLine();
	
				if ( null == line )
					break;
	
				line = line.trim();
	
				if ( !line.startsWith( "#" ) && !line.equals( "" ) )
					names.add( line );
			}
	
			is.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		return names;
	}
}

// kate: tab-width 4
