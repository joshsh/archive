/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.query.commands;

import info.aduna.iteration.CloseableIteration;

import org.openrdf.model.Literal;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.model.Value;
import org.openrdf.model.vocabulary.RDF;
import org.openrdf.model.vocabulary.XMLSchema;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;
import org.openrdf.sail.memory.MemoryStore;

import net.fortytwo.ripple.cli.ast.ListAST;
import net.fortytwo.ripple.cli.ast.StringAST;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.impl.sesame.SesameModel;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.LazyEvaluator;
import net.fortytwo.ripple.query.QueryEngine;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.URIMap;

public class DefinitionsTest extends RippleTestCase
{
	private class DefsTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Sail sail = new MemoryStore();
			sail.initialize();
			SailConnection sc = sail.getConnection();

			URIMap uriMap = new URIMap();
			Model model = new SesameModel( sail, uriMap );
			QueryEngine qe = new QueryEngine( model, new LazyEvaluator(), System.out, System.err );

			
			ListAST foobar = new ListAST( new StringAST( "foo"), new ListAST( new StringAST( "bar" ), new ListAST() ) );
			ListAST foobar2 = new ListAST( new StringAST( "foo2"), new ListAST( new StringAST( "bar2" ), new ListAST() ) );
			URI foobarUri = sail.getValueFactory().createURI( qe.getDefaultNamespace() + "foobar" );
			Literal foo = sail.getValueFactory().createLiteral( "foo", XMLSchema.STRING );
			Literal foo2 = sail.getValueFactory().createLiteral( "foo2", XMLSchema.STRING );
				
			Command defCmd = new DefineTermCmd( "foobar", foobar );
			Command undefCmd = new UndefineTermCmd( "foobar" );
			Command redefCmd = new RedefineTermCmd( "foobar", foobar2 );
		
			int count;
			Value obj;
			
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 0, count );

			// Undefine a term which has not been defined.
			qe.executeCommand( undefCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 0, count );
			
			// Define a term which is not yet defined.
			qe.executeCommand( defCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 3, count );
			obj = getSingleStatement( sc.getStatements( foobarUri, RDF.FIRST, null, false) ).getObject();
System.out.println("obj = " + obj);
			assertTrue( obj.equals( foo ) );
			assertFalse( obj.equals( foo2 ) );
			
			// Undefine a term which has been defined.
			qe.executeCommand( undefCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 0, count );			
			
			// Redefine a term which has already been defined.
			qe.executeCommand( defCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 3, count );
			obj = getSingleStatement( sc.getStatements( foobarUri, RDF.FIRST, null, false) ).getObject();
			assertTrue( obj.equals( foo ) );
			assertFalse( obj.equals( foo2 ) );
			qe.executeCommand( redefCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 3, count );
			obj = getSingleStatement( sc.getStatements( foobarUri, RDF.FIRST, null, false) ).getObject();
			assertTrue( obj.equals( foo2 ) );
			assertFalse( obj.equals( foo ) );
			
			// Undefine a term which has been redefined.
			qe.executeCommand( undefCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 0, count );
			
			// Redefine a term which has not yet been defined.
			qe.executeCommand( redefCmd );
			count = countStatements( sc.getStatements( foobarUri, null, null, false ) );
			assertEquals( 3, count );
			obj = getSingleStatement( sc.getStatements( foobarUri, RDF.FIRST, null, false) ).getObject();
			assertTrue( obj.equals( foo2 ) );
			assertFalse( obj.equals( foo ) );
			
			sc.close();
			sail.shutDown();			
		}
	}

	private int countStatements(final CloseableIteration<? extends Statement, SailException> iter) throws SailException {
		int count = 0;
		while (iter.hasNext()) {
			count++;
			iter.next();
		}
		iter.close();
		return count;
	}

	private Statement getSingleStatement(final CloseableIteration<? extends Statement, SailException> iter) throws SailException {
		Statement st = (iter.hasNext())	? iter.next() : null;
		iter.close();
		return st;
	}
	
	public void runTests() throws Exception
	{
		testAsynchronous( new DefsTest() );
	}
}
