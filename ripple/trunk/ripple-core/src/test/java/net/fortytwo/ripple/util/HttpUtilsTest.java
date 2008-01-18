package net.fortytwo.ripple.util;

import net.fortytwo.ripple.test.RippleTestCase;

import java.net.URI;

/**
 * Author: josh
 * Date: Jan 16, 2008
 * Time: 4:08:58 PM
 */
public class HttpUtilsTest extends RippleTestCase
{
    /**
     * Make sure that URI schemes are what we expect them to be
     */
    private class UriSchemTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			URI uri;

            uri = new URI( "http://example.com/foo" );
            assertEquals( "http", uri.getScheme() );

            uri = new URI( "jar:file:/tmp/foo.txt" );
            assertEquals( "jar", uri.getScheme() );
        }
	}


	public void runTests()
		throws Exception
	{
		testAsynchronous( new UriSchemTest() );
	}
}