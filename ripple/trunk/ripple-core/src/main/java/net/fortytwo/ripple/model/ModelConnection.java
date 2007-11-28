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
	void reset( boolean rollback ) throws RippleException;

	void commit() throws RippleException;

	boolean booleanValue( RippleValue rv ) throws RippleException;
	NumericLiteral numericValue( RippleValue rv )	throws RippleException;
	int intValue( RippleValue rv ) throws RippleException;
	String stringValue( RippleValue v ) throws RippleException;
	URI uriValue( RippleValue rv ) throws RippleException;

	RdfValue findSingleObject( RippleValue subj, RippleValue pred )	throws RippleException;
	RdfValue findAtLeastOneObject( RippleValue subj, RippleValue pred )	throws RippleException;
	RdfValue findAtMostOneObject( RippleValue subj, RippleValue pred ) throws RippleException;
	RdfValue findUniqueProduct( RippleValue subj, RippleValue pred ) throws RippleException;

	void copyStatements( RippleValue src, RippleValue dest ) throws RippleException;
	void removeStatementsAbout( URI subj ) throws RippleException;
	void putContainerMembers( RippleValue head, Sink<RippleValue> sink ) throws RippleException;

	void forget( RippleValue v ) throws RippleException;

	void findPredicates( RippleValue subject, Sink<RippleValue> sink ) throws RippleException;

// FIXME: Statements should not be part of the ModelConnection API
	void add( Statement st, Resource... contexts ) throws RippleException;
	void add( RippleValue subj, RippleValue pred, RippleValue obj ) throws RippleException;
	void remove( RippleValue subj, RippleValue pred, RippleValue obj ) throws RippleException;

// FIXME: URIs should not be part of the ModelConnection API
	void removeStatementsAbout( RdfValue subj, URI context ) throws RippleException;

// FIXME: Resources should not be part of the ModelConnection API
	public long countStatements( Resource context ) throws RippleException;

// FIXME: Sesame URIs should not be part of the ModelConnection API
	URI createUri( String s ) throws RippleException;
	URI createUri( String ns, String s ) throws RippleException;
	URI createUri( URI ns, String s ) throws RippleException;
// FIXME: Sesame literals should not be part of the ModelConnection API
	Literal createLiteral( String s ) throws RippleException;
	Literal createLiteral( String s, final String language ) throws RippleException;
	Literal createLiteral( String s, final URI dataType )	throws RippleException;
	Literal createLiteral( boolean b ) throws RippleException;
	Literal createLiteral( int i ) throws RippleException;
	Literal createLiteral( double d ) throws RippleException;
// FIXME: BNodes should not be part of the ModelConnection API
	BNode createBNode() throws RippleException;
	BNode createBNode( String id ) throws RippleException;
// FIXME: Statements should not be part of the ModelConnection API
	Statement createStatement( Resource subj, final URI pred, final Value obj ) throws RippleException;

	RippleValue createTypedLiteral( String value, final RippleValue type ) throws RippleException;
	
	RippleValue value( String s ) throws RippleException;
	RippleValue value( String s, String language ) throws RippleException;
// FIXME: this should use an implementation-independent URI class
	RippleValue value( String s, URI dataType ) throws RippleException;
	RippleValue value( boolean b ) throws RippleException;
	NumericLiteral value( int i ) throws RippleException;
	NumericLiteral value( long l ) throws RippleException;
	NumericLiteral value( double d ) throws RippleException;
// FIXME: this should use an implementation-independent URI class
	RippleValue value( URI uri );
	
	void setNamespace( String prefix, String ns, boolean override ) throws RippleException;
// FIXME: URIs should not be part of the ModelConnection interface
	void setNamespace( String prefix, URI ns, boolean override ) throws RippleException;

	void multiplyAsynch( RippleValue subj, RippleValue pred, Sink<RippleValue> sink ) throws RippleException;
	void multiply( RippleValue subj, RippleValue pred, Sink<RippleValue> sink ) throws RippleException;
	void divide( RippleValue obj, RippleValue pred, Sink<RippleValue> sink ) throws RippleException;

// FIXME: Namespaces should not be part of the ModelConnection API
	void getNamespaces( Sink<Namespace> sink ) throws RippleException;
// FIXME: Statements should not be part of the ModelConnection API
	void getStatements( RdfValue subj, RdfValue pred, RdfValue obj, Sink<Statement> sink ) throws RippleException;

	RdfSource getSource();

// TODO: Namespaces should not be part of the ModelConnection API
	void putNamespaces( Sink<Namespace> sink ) throws RippleException;

	void putContexts( Sink<RippleValue> sink ) throws RippleException;
	
	void exportNs( String ns, OutputStream os ) throws RippleException;
}
