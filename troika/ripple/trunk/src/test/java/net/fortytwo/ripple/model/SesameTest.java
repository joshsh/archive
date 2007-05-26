package net.fortytwo.ripple.model;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import net.fortytwo.ripple.test.RippleTestCase;

import org.apache.log4j.Logger;

import org.openrdf.model.Resource;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;
import org.openrdf.rio.RDFFormat;
import org.openrdf.repository.Repository;
import org.openrdf.repository.RepositoryConnection;
import org.openrdf.repository.RepositoryResult;
import org.openrdf.repository.sail.SailRepository;
import org.openrdf.sail.memory.MemoryStore;

public class SesameTest extends RippleTestCase
{
	private final static Logger s_logger = Logger.getLogger( SesameTest.class );

	static int countStatements( RepositoryConnection rc, URI context )
		throws Exception
	{
		int count = 0;

		RepositoryResult<Statement> stmtIter
			= ( null == context )
				? rc.getStatements( null, null, null, false )
				: rc.getStatements( null, null, null, false, context );

		while ( stmtIter.hasNext() )
		{
			stmtIter.next();
			count++;
		}

		stmtIter.close();

		return count;
	}

	static int countObjects( RepositoryConnection rc, Resource subj, URI pred, URI context )
		throws Exception
	{
		int count = 0;

		RepositoryResult<Statement> stmtIter
			= ( null == context )
				? rc.getStatements( subj, pred, null, false )
				: rc.getStatements( subj, pred, null, false, context );

		while ( stmtIter.hasNext() )
		{
			stmtIter.next();
			count++;
		}

		stmtIter.close();

		return count;
	}

	private class RecoverFromParseErrorTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Repository repo = new SailRepository(
				new MemoryStore() );
			repo.initialize();
			RepositoryConnection rc = repo.getConnection();

			String bad = "bad" ;
			String good = "@prefix foo:  <http://example.org/foo#>.\n"
				+ "foo:a foo:b foo:c." ;

			InputStream is;

			try {
				is = new ByteArrayInputStream( bad.getBytes() );
				rc.add( is, "", RDFFormat.TURTLE );
			} catch ( Exception e ) {}
			try {
				is = new ByteArrayInputStream( good.getBytes() );
				rc.add( is, "", RDFFormat.TURTLE );
			} catch ( Exception e ) {}

			assertEquals( 1, countStatements( rc, null ) );

			rc.close();
			repo.shutDown();
		}
	}

	private class AddFromInputStreamTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Repository repo = new SailRepository(
				new MemoryStore() );
			repo.initialize();
			RepositoryConnection rc = repo.getConnection();

			URI ctxA = repo.getValueFactory().createURI( "urn:test.AddFromInputStreamTest.ctxA#" );

			String s = "@prefix foo:  <http://example.org/foo#>.\n"
				+ "foo:a foo:b foo:c." ;
			InputStream is = new ByteArrayInputStream( s.getBytes() );

			rc.add( is, ctxA.toString(), RDFFormat.TURTLE, ctxA );

			assertEquals( 1, countStatements( rc, null ) );
			assertEquals( 1, countStatements( rc, ctxA ) );

			rc.close();
			repo.shutDown();
		}
	}

	private class DuplicateStatementsTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Repository repo = new SailRepository(
				new MemoryStore() );
			repo.initialize();
			RepositoryConnection rc = repo.getConnection();

			URI ctxA = repo.getValueFactory().createURI( "urn:test.DuplicateStatementsTest.ctxA#" );
			URI ctxB = repo.getValueFactory().createURI( "urn:test.DuplicateStatementsTest.ctxB#" );

			URI uri = repo.getValueFactory().createURI( "http://example.com/uri" );

			rc.add( uri, uri, uri, ctxA );
			rc.add( uri, uri, uri, ctxB );

			assertEquals( 1, countStatements( rc, ctxA ) );
			assertEquals( 1, countStatements( rc, ctxB ) );

			// FAIL!
			assertEquals( 1, countStatements( rc, null ) );

			rc.close();
			repo.shutDown();
		}
	}

	private class DuplicateStatementsTest2 extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Repository repo = new SailRepository(
				new MemoryStore() );
			repo.initialize();
			RepositoryConnection rc = repo.getConnection();

			URI ctxA = repo.getValueFactory().createURI( "urn:test.DuplicateStatementsTest2.ctxA#" );
			URI ctxB = repo.getValueFactory().createURI( "urn:test.DuplicateStatementsTest2.ctxB#" );
			URI a = repo.getValueFactory().createURI( "http://example.org/foo#a" );
			URI b = repo.getValueFactory().createURI( "http://example.org/foo#b" );

			InputStream is;
			String s = "@prefix foo:  <http://example.org/foo#>.\n"
				+ "foo:a foo:b foo:c." ;
			is = new ByteArrayInputStream( s.getBytes() );
			rc.add( is, ctxA.toString(), RDFFormat.TURTLE, ctxA );
			is = new ByteArrayInputStream( s.getBytes() );
			rc.add( is, ctxB.toString(), RDFFormat.TURTLE, ctxB );

//			assertEquals( 1, countObjects( rc, a, b, ctxA ) );
//			assertEquals( 1, countObjects( rc, a, b, ctxB ) );

			// FAIL!
			assertEquals( 1, countObjects( rc, a, b, null ) );

			rc.close();
			repo.shutDown();
		}
	}

	public void runTests()
		throws Throwable
	{
		// Note: bug fixed in Sesame2-beta3:
		//    http://www.openrdf.org/issues/browse/SES-358?watch=true
		testSynchronous( new AddFromInputStreamTest() );

		// Note: bug fixed in Sesame2-beta3:
		//    http://www.openrdf.org/forum/mvnforum/viewthread?thread=1229
		testSynchronous( new RecoverFromParseErrorTest() );

// See: http://www.openrdf.org/forum/mvnforum/viewthread?thread=1320
//		testSynchronous( new DuplicateStatementsTest() );
//		testSynchronous( new DuplicateStatementsTest2() );
	}
}

// kate: tab-width 4
