package net.fortytwo.ripple.tools;

import java.io.File;

import java.text.DateFormat;
import java.text.SimpleDateFormat;

import java.util.Date;

import org.openrdf.model.Statement;
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
		fileNs = "http://fortytwo.net/2007/02/06/file#",
		rdfNs = "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
		rdfsNs = "http://www.w3.org/2000/01/rdf-schema#",
		xsdNs = "http://www.w3.org/2001/XMLSchema#";

	private int fileCount = 0;

	public FileRdfizer( ValueFactory valueFactory )
	{
		this.valueFactory = valueFactory;
		
		fileUri         = valueFactory.createURI( fileNs + "File" );
		directoryUri    = valueFactory.createURI( fileNs + "Directory" );
		nameUri         = valueFactory.createURI( fileNs + "name" );
		urlUri          = valueFactory.createURI( fileNs + "url" );
		parentOfUri     = valueFactory.createURI( fileNs + "parentOf" );
		childOfUri      = valueFactory.createURI( fileNs + "childOf" );
		sizeUri         = valueFactory.createURI( fileNs + "size" );
		isHiddenUri     = valueFactory.createURI( fileNs + "isHidden" );
		lastModifiedUri = valueFactory.createURI( fileNs + "lastModified" );

		propertyUri     = valueFactory.createURI( rdfNs + "Property" );
		typeUri         = valueFactory.createURI( rdfNs + "type" );

		classUri        = valueFactory.createURI( rdfsNs + "Class" );
		domainUri       = valueFactory.createURI( rdfsNs + "domain" );
		rangeUri        = valueFactory.createURI( rdfsNs + "range" );
		subClassOfUri   = valueFactory.createURI( rdfsNs + "subClassOf" );

		booleanUri      = valueFactory.createURI( xsdNs + "boolean" );
		dateTimeUri     = valueFactory.createURI( xsdNs + "dateTime" );
		longUri         = valueFactory.createURI( xsdNs + "long" );
		stringUri       = valueFactory.createURI( xsdNs + "string" );
	}

	public void addTree( File file, URI context, String namespace, RepositoryConnection connection )
		throws org.openrdf.sail.SailException,
		org.openrdf.repository.RepositoryException
	{
		connection.setNamespace( "file", fileNs );
		connection.setNamespace( "xsd", xsdNs );

		add( file, context, namespace, connection );
	}

	private URI add( File file, URI context, String namespace, RepositoryConnection connection )
		throws org.openrdf.sail.SailException,
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

		catch ( java.net.MalformedURLException e ) {}

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

	private URI createFileUri( File file, String namespace )
	{
//		return valueFactory.createURI( file.toURI().toString() );
		return valueFactory.createURI( namespace, "file" + fileCount++ );
	}
}
