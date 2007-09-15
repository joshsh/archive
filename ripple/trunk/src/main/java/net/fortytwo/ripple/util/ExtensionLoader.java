/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.util;

import java.io.InputStream;

import java.util.Collection;
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

	private Collection<String> getNames() throws RippleException
	{
		try
		{
			InputStream is
				= net.fortytwo.ripple.Ripple.class.getResourceAsStream(
					"extensions.txt" );
			Collection<String> names = FileUtils.getLines( is );
			is.close();

			return names;
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}
}

// kate: tab-width 4
