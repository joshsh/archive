/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import java.io.FileOutputStream;
import java.io.OutputStream;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.rdf.BNodeClosureFilter;
import net.fortytwo.ripple.rdf.SesameOutputAdapter;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.rdf.RdfUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

public class ExportNsCmd extends Command
{
	private String nsPrefix, fileName;

	public ExportNsCmd( final String nsPrefix, final String fileName )
	{
		this.nsPrefix = nsPrefix;
		this.fileName = fileName;
	}

	public void execute( final QueryEngine qe, final ModelConnection mc )
		throws RippleException
	{
		OutputStream out;

		String ns = qe.getLexicon().resolveNamespacePrefix( nsPrefix );
		if ( null == ns )
		{
			throw new RippleException( "namespace prefix '" + nsPrefix + "' is not defined" );
		}

		try
		{
			out = new FileOutputStream( fileName );
		}

		catch ( java.io.FileNotFoundException e )
		{
			throw new RippleException( e );
		}

		exportNs( ns, out, mc );

		try
		{
			out.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		System.out.println( "\nExported namespace " + nsPrefix + " to " + fileName + "\n" );
	}

	protected void abort()
	{
	}

	private void exportNs( final String ns, final OutputStream os, final ModelConnection mc )
		throws RippleException
	{
		SesameOutputAdapter adapter = RdfUtils.createOutputAdapter( os, Ripple.exportFormat() );

		final Sink<Resource> bnodeClosure = new BNodeClosureFilter(
			adapter.statementSink(), mc.getSailConnection() );

		// Hackishly find all terms in the given namespace which are the subject
		// of statements.
		Sink<Statement> sink = new Sink<Statement>()
		{
			public void put( final Statement st ) throws RippleException
			{
				Resource subj = st.getSubject();
				if ( subj instanceof URI
						&& subj.toString().startsWith( ns ) )
				{
					bnodeClosure.put( subj );
				}
			}
		};

		Buffer<Statement> buffer = new Buffer<Statement>( sink );
		mc.getStatements( null, null, null, buffer );

		adapter.startRDF();
		buffer.flush();
		adapter.endRDF();
	}
}

// kate: tab-width 4
