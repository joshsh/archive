/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.linkeddata;

import info.aduna.iteration.CloseableIteration;

import net.fortytwo.ripple.test.RippleTestCase;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Statement;
import org.openrdf.model.ValueFactory;
import org.openrdf.sail.Sail;
import org.openrdf.sail.SailConnection;
import org.openrdf.sail.SailException;

public class LinkedDataSailConnectionTest extends RippleTestCase
{
	private class CountStatementsTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Sail sail = getTestSail();
			ValueFactory vf = sail.getValueFactory();
			SailConnection sc = sail.getConnection();

			URI ctxA = vf.createURI( "urn:org.example.test.countStatementsTest#" );
			URI uri1 = vf.createURI( "urn:org.example.test#uri1" );
			URI uri2 = vf.createURI( "urn:org.example.test#uri2" );
			URI uri3 = vf.createURI( "urn:org.example.test#uri3" );

			URI [] uris = {uri1, uri2, uri3};

			assertEquals( 0, countStatements( sc, ctxA ) );
			for ( int i = 0; i < 3; i++ )
			{
				for ( int j = 0; j < 3; j++ )
				{
					for ( int k = 0; k < 3; k++ )
					{
						sc.addStatement(
							uris[i], uris[j], uris[k], ctxA );
					}
				}
			}
			sc.commit();
			assertEquals( 27, countStatements( sc, ctxA ) );

			sc.close();
		}
	}

	private static long countStatements( final SailConnection sc, final Resource... contexts )
		throws SailException
	{
		CloseableIteration<? extends Statement, SailException> iter
			= sc.getStatements( null, null, null, false, contexts );

		long count = 0;
		while ( iter.hasNext() )
		{
			iter.next();
			count++;
		}

		return count;
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new CountStatementsTest() );
	}
}

// kate: tab-width 4
