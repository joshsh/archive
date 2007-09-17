/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.tools;

import java.io.File;

import java.text.DateFormat;
import java.text.SimpleDateFormat;

import java.util.Date;

import org.openrdf.model.URI;
import org.openrdf.model.ValueFactory;
import org.openrdf.repository.RepositoryConnection;

public class FileRdfizer
{
	private URI
		fileUri,
		directoryUri,
		nameUri,
		urlUri,
		parentOfUri,
		childOfUri,
		sizeUri,
		isHiddenUri,
		lastModifiedUri;

	private URI
		propertyUri,
		typeUri;

	private URI
		classUri,
		domainUri,
		rangeUri,
		subClassOfUri;

	private URI
		booleanUri,
		dateTimeUri,
		longUri,
		stringUri;
	
	private ValueFactory valueFactory;

	private static DateFormat xsdDateTimeFormat
		= new SimpleDateFormat( "yyyy-MM-dd'T'HH:mm:ss" );

	private static final String
		FILE = "http://fortytwo.net/2007/02/06/file#",
		RDF = "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
		RDFS = "http://www.w3.org/2000/01/rdf-schema#",
		XSD = "http://www.w3.org/2001/XMLSchema#";

	private int fileCount = 0;

	public FileRdfizer( final ValueFactory valueFactory )
	{
		this.valueFactory = valueFactory;
		
		fileUri         = valueFactory.createURI( FILE + "File" );
		directoryUri    = valueFactory.createURI( FILE + "Directory" );
		nameUri         = valueFactory.createURI( FILE + "name" );
		urlUri          = valueFactory.createURI( FILE + "url" );
		parentOfUri     = valueFactory.createURI( FILE + "parentOf" );
		childOfUri      = valueFactory.createURI( FILE + "childOf" );
		sizeUri         = valueFactory.createURI( FILE + "size" );
		isHiddenUri     = valueFactory.createURI( FILE + "isHidden" );
		lastModifiedUri = valueFactory.createURI( FILE + "lastModified" );

		propertyUri     = valueFactory.createURI( RDF + "Property" );
		typeUri         = valueFactory.createURI( RDF + "type" );

		classUri        = valueFactory.createURI( RDFS + "Class" );
		domainUri       = valueFactory.createURI( RDFS + "domain" );
		rangeUri        = valueFactory.createURI( RDFS + "range" );
		subClassOfUri   = valueFactory.createURI( RDFS + "subClassOf" );

		booleanUri      = valueFactory.createURI( XSD + "boolean" );
		dateTimeUri     = valueFactory.createURI( XSD + "dateTime" );
		longUri         = valueFactory.createURI( XSD + "long" );
		stringUri       = valueFactory.createURI( XSD + "string" );
	}

	public void addTree( final File file,
						final URI context,
						final String namespace,
						final RepositoryConnection connection )
		throws org.openrdf.sail.SailException,
		org.openrdf.repository.RepositoryException
	{
		connection.setNamespace( "file", FILE );
		connection.setNamespace( "xsd", XSD );

		add( file, context, namespace, connection );
	}

	private URI add( final File file,
					final URI context,
					final String namespace,
					final RepositoryConnection connection )
		throws
			org.openrdf.sail.SailException,
			org.openrdf.repository.RepositoryException
	{
		URI self = createFileUri( file, namespace );

		// type
		connection.add( valueFactory.createStatement(
			self,
			typeUri,
			file.isDirectory() ? directoryUri : fileUri,
			context ) );

		// name
		String name = file.getName();
		connection.add( valueFactory.createStatement(
			self,
			nameUri,
			valueFactory.createLiteral( name, stringUri ),
			context ) );
		
		// url
		try
		{
			String url = file.toURL().toString();
			connection.add( valueFactory.createStatement(
				self,
				urlUri,
				valueFactory.createLiteral( url, stringUri ),
				context ) );
		}

		catch ( java.net.MalformedURLException e )
		{
			return null;
		}

		// size
		long size = file.length();
		connection.add( valueFactory.createStatement(
			self,
			sizeUri,
			valueFactory.createLiteral( "" + size, longUri ),
			context ) );

		// isHidden
		boolean isHidden = file.isHidden();
		connection.add( valueFactory.createStatement(
			self,
			isHiddenUri,
			valueFactory.createLiteral( "" + isHidden, booleanUri ),
			context ) );

		// lastModified
		long lastModified = file.lastModified();
		connection.add( valueFactory.createStatement(
			self,
			lastModifiedUri,
			valueFactory.createLiteral( xsdDateTimeFormat.format(
				new Date( lastModified ) ), dateTimeUri ),
			context ) );

		// parentOf
		if ( file.isDirectory() )
		{
			File [] children = file.listFiles();
			for ( File child : children )
			{
				URI childUri = add( child, context, namespace, connection );
				connection.add( valueFactory.createStatement(
					self,
					parentOfUri,
					childUri,
					//createFileUri( child, namespace ),
					context ) );
				connection.add( valueFactory.createStatement(
					childUri,
					childOfUri,
					self,
					context ) );
			}
		}

		return self;
	}

	private URI createFileUri( final File file, final String namespace )
	{
//		return valueFactory.createURI( file.toURI().toString() );
		return valueFactory.createURI( namespace, "file" + fileCount++ );
	}
}

// kate: tab-width 4
