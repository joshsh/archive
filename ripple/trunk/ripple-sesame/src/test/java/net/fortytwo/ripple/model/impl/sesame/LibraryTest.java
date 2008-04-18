/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.model.impl.sesame;

import net.fortytwo.ripple.model.ModelBridge;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveStackMapping;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.test.RippleTestCase;

public class LibraryTest extends RippleTestCase
{
	private class PrimitiveAliasTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			ModelConnection mc = getTestModel().getConnection( "for PrimitiveAliasTest" );

			RdfValue dup05 = new RdfValue(
				mc.createUri( "http://fortytwo.net/2007/05/ripple/stack#dup" ) );
			RdfValue dup08 = new RdfValue(
				mc.createUri( "http://fortytwo.net/2007/08/ripple/stack#dup" ) );

			ModelBridge bridge = mc.getModel().getBridge();

			RippleValue dup05Val = bridge.get( dup05 );
			RippleValue dup08Val = bridge.get( dup08 );

			assertNotNull( dup05Val );
			assertNotNull( dup08Val );
			assertTrue( dup05Val instanceof PrimitiveStackMapping);
			assertTrue( dup08Val instanceof PrimitiveStackMapping);

			assertEquals( dup05Val, dup08Val );

			mc.close();
		}
	}

	public void runTests()
		throws Exception
	{
		testSynchronous( new PrimitiveAliasTest() );
	}
}
