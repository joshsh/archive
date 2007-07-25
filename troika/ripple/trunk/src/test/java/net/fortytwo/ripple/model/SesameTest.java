package net.fortytwo.ripple.model;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import net.fortytwo.ripple.test.RippleTestCase;

import org.apache.log4j.Logger;

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

s_logger.debug( "### start" );
			try {
				is = new ByteArrayInputStream( bad.getBytes() );
				rc.add( is, "", RDFFormat.TURTLE );
			} catch ( Exception e ) {}
s_logger.debug( "### mid" );
			try {
				is = new ByteArrayInputStream( good.getBytes() );
				rc.add( is, "", RDFFormat.TURTLE );
			} catch ( Exception e ) {}
s_logger.debug( "### stop" );

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
/* 60 */    assertEquals( 1, countStatements( rc, ctxA ) );

			rc.close();
			repo.shutDown();
		}
	}

	public void runTests()
		throws Exception
	{
		// Note: bug fixed in Sesame2-beta3:
		//    http://www.openrdf.org/issues/browse/SES-358?watch=true
//		testSynchronous( new AddFromInputStreamTest() );

		// Note: bug fixed in Sesame2-beta3:
		//    http://www.openrdf.org/forum/mvnforum/viewthread?thread=1229
//		testSynchronous( new RecoverFromParseErrorTest() );

	}
}

// kate: tab-width 4
