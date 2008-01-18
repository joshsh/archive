/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.io;

import java.util.HashSet;
import java.util.Set;

import net.fortytwo.ripple.RippleException;

import org.apache.log4j.Logger;
import org.restlet.resource.Representation;

// Note: throughout this implementation, both the caching context of a URI and
//       its associated web location are the same as its success or failure 'memo'.
public class HttpUriDereferencer implements UriDereferencer
{
	private static final Logger LOGGER = Logger.getLogger( HttpUriDereferencer.class );

	private static final String[] BADEXT = {
		"123", "3dm", "3dmf", "3gp", "8bi", "aac", "ai", "aif", "app", "asf",
		"asp", "asx", "avi", "bat", "bin", "bmp", "c", "cab", "cfg", "cgi",
		"com", "cpl", "cpp", "css", "csv", "dat", "db", "dll", "dmg", "dmp",
		"doc", "drv", "drw", "dxf", "eps", "exe", "fnt", "fon", "gif", "gz",
		"h", "hqx", "htm", "html", "iff", "indd", "ini", "iso", "java", "jpeg",
		"jpg", "js", "jsp", "key", "log", "m3u", "mdb", "mid", "midi", "mim",
		"mng", "mov", "mp3", "mp4", "mpa", "mpg", "msg", "msi", "otf", "pct",
		"pdf", "php", "pif", "pkg", "pl", "plugin", "png", "pps", "ppt", "ps",
		"psd", "psp", "qt", "qxd", "qxp", "ra", "ram", "rar", "reg", "rm",
		"rtf", "sea", "sit", "sitx", "sql", "svg", "swf", "sys", "tar", "tif",
		"ttf", "uue", "vb", "vcd", "wav", "wks", "wma", "wmv", "wpd", "wps",
		"ws", "xhtml", "xll", "xls", "yps", "zip"};

	private Set<String> badExtensions;

	public HttpUriDereferencer()
	{
		badExtensions = new HashSet<String>();

		for ( int i = 0; i < BADEXT.length; i++ )
		{
			badExtensions.add( BADEXT[i] );
		}
	}

	public Representation handle( final String uri ) throws RippleException
	{
	    // Don't dereference a URI which appears to point to a file which is not
		// an RDF document.
		int l = uri.lastIndexOf( '.' );
		if ( l >= 0 && badExtensions.contains( uri.substring( l + 1 ) ) )
		{
			return null;
		}

        return new HttpRepresentation( uri );
	}
}

// kate: tab-width 4
