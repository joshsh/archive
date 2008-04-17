/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model;

import java.io.OutputStream;
import java.util.Date;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.Sink;
import net.fortytwo.ripple.rdf.RDFSource;

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

	boolean toBoolean( RippleValue v ) throws RippleException;
	NumericValue toNumericValue( RippleValue v )	throws RippleException;
    Date toDateValue( RippleValue v ) throws RippleException;

    /**
	 *
	 * @param v
	 * @return the string representation of a value.  This is not identical to
	 * Object.toString(), and may involve a loss of information.
	 * @throws RippleException
	 */
	String toString( RippleValue v ) throws RippleException;

	URI toUri( RippleValue v ) throws RippleException;
	void toList( RippleValue v, Sink<RippleList, RippleException> sink ) throws RippleException;

	RdfValue findSingleObject( RippleValue subj, RippleValue pred )	throws RippleException;
	RdfValue findAtLeastOneObject( RippleValue subj, RippleValue pred )	throws RippleException;
	RdfValue findAtMostOneObject( RippleValue subj, RippleValue pred ) throws RippleException;
	RdfValue findUniqueProduct( RippleValue subj, RippleValue pred ) throws RippleException;

	void copyStatements( RippleValue src, RippleValue dest ) throws RippleException;
	void removeStatementsAbout( URI subj ) throws RippleException;
	void putContainerMembers( RippleValue head, Sink<RippleValue, RippleException> sink ) throws RippleException;

	void forget( RippleValue v ) throws RippleException;

	void findPredicates( RippleValue subject, Sink<RippleValue, RippleException> sink ) throws RippleException;

// FIXME: Statements should not be part of the ModelConnection API
	void add( Statement st, Resource... contexts ) throws RippleException;
	void add( RippleValue subj, RippleValue pred, RippleValue obj ) throws RippleException;
	void remove( RippleValue subj, RippleValue pred, RippleValue obj ) throws RippleException;

// FIXME: URIs should not be part of the ModelConnection API
	void removeStatementsAbout( RdfValue subj, URI context ) throws RippleException;

// FIXME: Resources should not be part of the ModelConnection API
	public long countStatements( Resource... contexts ) throws RippleException;

// FIXME: Sesame URIs should not be part of the ModelConnection API
	URI createUri( String s ) throws RippleException;
	URI createUri( String ns, String s ) throws RippleException;
	URI createUri( URI ns, String s ) throws RippleException;
// FIXME: BNodes should not be part of the ModelConnection API
	Resource createBNode() throws RippleException;
	Resource createBNode( String id ) throws RippleException;
// FIXME: Statements should not be part of the ModelConnection API
	Statement createStatement( Resource subj, final URI pred, final Value obj ) throws RippleException;

	RippleValue createTypedLiteral( String value, final RippleValue type ) throws RippleException;
	
	RippleValue value( String s ) throws RippleException;
	RippleValue value( String s, String language ) throws RippleException;
// FIXME: this should use an implementation-independent URI class
	RippleValue value( String s, URI dataType ) throws RippleException;
	RippleValue value( boolean b ) throws RippleException;
	NumericValue value( int i ) throws RippleException;
	NumericValue value( long l ) throws RippleException;
	NumericValue value( double d ) throws RippleException;
// FIXME: this should use an implementation-independent URI class
	RippleValue value( URI uri );
	
    RippleList list();
    RippleList list( RippleValue v );
	RippleList list( RippleValue v, RippleList rest );
	RippleList invert( RippleList l );
	RippleList concat( RippleList head, RippleList tail );
	
	void setNamespace( String prefix, String ns, boolean override ) throws RippleException;

    void query( GetStatementsQuery query, Sink<RippleValue, RippleException> sink ) throws RippleException;
    void queryAsynch( GetStatementsQuery query, Sink<RippleValue, RippleException> sink ) throws RippleException;
	void multiplyAsynch( RippleValue subj, RippleValue pred, Sink<RippleValue, RippleException> sink, boolean includeInferred ) throws RippleException;
	void multiply( RippleValue subj, RippleValue pred, Sink<RippleValue, RippleException> sink, boolean includeInferred ) throws RippleException;
	void divide( RippleValue obj, RippleValue pred, Sink<RippleValue, RippleException> sink ) throws RippleException;

// FIXME: Namespaces should not be part of the ModelConnection API
	void getNamespaces( Sink<Namespace, RippleException> sink ) throws RippleException;
// FIXME: Statements should not be part of the ModelConnection API
	void getStatements( RdfValue subj, RdfValue pred, RdfValue obj, Sink<Statement, RippleException> sink, boolean includeInferred ) throws RippleException;

	RDFSource getSource();

// TODO: Namespaces should not be part of the ModelConnection API
	void putNamespaces( Sink<Namespace, RippleException> sink ) throws RippleException;

	void putContexts( Sink<RippleValue, RippleException> sink ) throws RippleException;
	
	void exportNamespace( String ns, OutputStream os ) throws RippleException;
}
