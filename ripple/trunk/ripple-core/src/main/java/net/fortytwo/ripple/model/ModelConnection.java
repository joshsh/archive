/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.OutputStream;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.rdf.RdfSource;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.BNode;
import org.openrdf.model.Literal;
import org.openrdf.model.Namespace;
import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

public interface ModelConnection
{
	Model getModel();
	String getName();
	
	void close() throws RippleException;

	/**
	*  Returns the ModelConnection to a normal state after an Exception has
	*  been thrown.
	*/
	void reset( final boolean rollback ) throws RippleException;

	void commit() throws RippleException;

	boolean booleanValue( final RippleValue rv ) throws RippleException;
	NumericLiteral numericValue( final RippleValue rv )	throws RippleException;
	int intValue( final RippleValue rv ) throws RippleException;
	String stringValue( final RippleValue v ) throws RippleException;
	URI uriValue( final RippleValue rv ) throws RippleException;

	RdfValue findSingleObject( final RippleValue subj, final RippleValue pred )	throws RippleException;
	RdfValue findAtLeastOneObject( final RippleValue subj, final RippleValue pred )	throws RippleException;
	RdfValue findAtMostOneObject( final RippleValue subj, final RippleValue pred ) throws RippleException;
	RdfValue findUniqueProduct( final RippleValue subj, final RippleValue pred ) throws RippleException;

	void copyStatements( final RippleValue src, final RippleValue dest ) throws RippleException;
	void removeStatementsAbout( final URI subj ) throws RippleException;
	void putContainerMembers( final RippleValue head, final Sink<RippleValue> sink ) throws RippleException;

	void forget( final RippleValue v ) throws RippleException;

	void findPredicates( final RippleValue subject,	final Sink<RippleValue> sink ) throws RippleException;

// FIXME: Statements should be absent from the ModelConnection API
	void add( final Statement st, final Resource... contexts ) throws RippleException;
	void add( final RippleValue subj, final RippleValue pred, final RippleValue obj ) throws RippleException;
	void remove( final RippleValue subj, final RippleValue pred, final RippleValue obj ) throws RippleException;

// FIXME: URIs should be absent from the ModelConnection API
	void removeStatementsAbout( final RdfValue subj, final URI context ) throws RippleException;

// FIXME: Resources should be absent from the ModelConnection API
	public long countStatements( final Resource context ) throws RippleException;

// FIXME: Sesame URIs should be absent from the ModelConnection API
	URI createUri( final String s ) throws RippleException;
	URI createUri( final String ns, final String s ) throws RippleException;
	URI createUri( final URI ns, final String s ) throws RippleException;
// FIXME: Sesame literals should be absent from the ModelConnection API
	Literal createLiteral( final String s ) throws RippleException;
	Literal createLiteral( final String s, final String language ) throws RippleException;
	Literal createLiteral( final String s, final URI dataType )	throws RippleException;
	Literal createLiteral( final boolean b ) throws RippleException;
	Literal createLiteral( final int i ) throws RippleException;
	Literal createLiteral( final double d ) throws RippleException;
// FIXME: BNodes should be absent from the ModelConnection API
	BNode createBNode() throws RippleException;
	BNode createBNode( final String id ) throws RippleException;
// FIXME: Statements should be absent from the ModelConnection API
	Statement createStatement( final Resource subj, final URI pred, final Value obj ) throws RippleException;

	RdfValue createTypedLiteral( final String value, final RippleValue type ) throws RippleException;
	
	RdfValue createValue( final String s ) throws RippleException;
	RdfValue createValue( final String s, final String language ) throws RippleException;
	RdfValue createValue( final String s, final URI dataType ) throws RippleException;
	RdfValue createValue( final boolean b ) throws RippleException;
	RdfValue createValue( final int i ) throws RippleException;
	RdfValue createValue( final long l ) throws RippleException;
	RdfValue createValue( final double d ) throws RippleException;

	void setNamespace( final String prefix, final String ns, final boolean override ) throws RippleException;
// FIXME: URIs should be absent from the ModelConnection interface
	void setNamespace( final String prefix, final URI ns, final boolean override ) throws RippleException;

	void multiplyAsynch( final RippleValue subj, final RippleValue pred, final Sink<RippleValue> sink ) throws RippleException;
	void multiply( final RippleValue subj, final RippleValue pred, final Sink<RippleValue> sink ) throws RippleException;
	void divide( final RippleValue obj, final RippleValue pred, final Sink<RippleValue> sink ) throws RippleException;

// FIXME: Namespaces should be absent from the ModelConnection API
	void getNamespaces( final Sink<Namespace> sink ) throws RippleException;
// FIXME: Statements should be absent from the ModelConnection API
	void getStatements( final RdfValue subj,
								final RdfValue pred,
								final RdfValue obj,
								final Sink<Statement> sink ) throws RippleException;

	RdfSource getSource();

// TODO: Namespaces should not be part of the ModelConnection API
	void putNamespaces( final Sink<Namespace> sink ) throws RippleException;

	void putContexts( final Sink<RippleValue> sink ) throws RippleException;
	
	void exportNs( final String ns, final OutputStream os ) throws RippleException;
}
