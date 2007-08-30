package net.fortytwo.ripple.model;

import java.io.InputStream;

import java.util.Collection;

import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.FileUtils;

public class LexiconUpdaterTest extends RippleTestCase
{
	private class RejectBadNamespaces extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Lexicon lexicon = new Lexicon( getTestModel() );

			Iterator<String> badPrefixIter = getLines( "badNsPrefixes.txt" ).iterator();
			assertTrue( badPrefixIter.hasNext() );
			while ( badPrefixIter.hasNext() )
			{
				String prefix = badPrefixIter.next();
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
	}
}

// kate: tab-width 4
