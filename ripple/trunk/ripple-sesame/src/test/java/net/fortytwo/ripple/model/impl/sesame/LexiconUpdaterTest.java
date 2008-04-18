/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model.impl.sesame;

import java.io.InputStream;
import java.util.Collection;
import java.util.Iterator;

import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.FileUtils;
import net.fortytwo.ripple.model.LexiconUpdater;
import net.fortytwo.ripple.model.Lexicon;

import org.openrdf.model.Namespace;
import org.openrdf.model.impl.NamespaceImpl;

public class LexiconUpdaterTest extends RippleTestCase
{
	private class RejectBadNamespaces extends TestRunnable
	{
		public void test()
			throws Exception
		{
			String nsBase = "http://example.org/ns";
			int i = 0;

			Lexicon lexicon = new Lexicon( getTestModel() );
			LexiconUpdater updater = new LexiconUpdater( lexicon );

			Iterator<String> prefixIter = getLines( "badNsPrefixes.txt" ).iterator();
			assertTrue( prefixIter.hasNext() );
			while ( prefixIter.hasNext() )
			{
				String prefix = prefixIter.next();
				assertNull( lexicon.resolveNamespacePrefix( prefix ) );

				i++;
				String nsUri = nsBase + i + "#";
				Namespace ns = new NamespaceImpl( prefix, nsUri );
				updater.adderSink().namespaceSink().put( ns );

				assertNull( lexicon.resolveNamespacePrefix( prefix ) );
			}
		}
	}

	private class AcceptGoodNamespaces extends TestRunnable
	{
		public void test()
			throws Exception
		{
			String nsBase = "http://example.org/ns";
			int i = 0;

			Lexicon lexicon = new Lexicon( getTestModel() );
			LexiconUpdater updater = new LexiconUpdater( lexicon );

			Iterator<String> prefixIter = getLines( "goodNsPrefixes.txt" ).iterator();
			assertTrue( prefixIter.hasNext() );
			while ( prefixIter.hasNext() )
			{
				String prefix = prefixIter.next();
				assertNull( lexicon.resolveNamespacePrefix( prefix ) );

				i++;
				String nsUri = nsBase + i + "#";
				Namespace ns = new NamespaceImpl( prefix, nsUri );
				updater.adderSink().namespaceSink().put( ns );

				assertEquals( lexicon.resolveNamespacePrefix( prefix ), nsUri );
			}
		}
	}

	Collection<String> getLines( final String fileName ) throws Exception
	{
		InputStream is = LexiconUpdaterTest.class.getResourceAsStream( fileName );
		Collection<String> lines = FileUtils.getLines( is );
		is.close();
		return lines;
	}

	public void runTests()
		throws Exception
	{
		testAsynchronous( new RejectBadNamespaces() );
		testAsynchronous( new AcceptGoodNamespaces() );
	}
}

// kate: tab-width 4
