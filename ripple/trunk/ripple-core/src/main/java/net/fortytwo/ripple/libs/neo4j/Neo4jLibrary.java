/*
 * $URL: http://svn.fortytwo.net/projects/ripple/trunk/ripple-core/src/main/java/net/fortytwo/ripple/libs/services/ServicesLibrary.java $
 * $Revision: 981 $
 * $Author: josh $
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.libs.neo4j;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Library;
import net.fortytwo.ripple.UriMap;

// kate: tab-width 4
/**
 * A collection of hooks into selected web services.
 */
public class Neo4jLibrary extends Library
{
	private static final String NS = "http://fortytwo.net/2007/08/ripple/neo4j#";

	public void load( final UriMap uf, final ModelConnection mc )
		throws RippleException
	{
		uf.put( NS, getClass().getResource( "neo4j.ttl" ) + "#" );

        registerPrimitive( GetProperty.class, NS + "getProperty", mc );
	}
}