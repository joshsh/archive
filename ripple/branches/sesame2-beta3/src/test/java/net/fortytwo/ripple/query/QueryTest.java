package net.fortytwo.ripple.model;

import java.io.PipedInputStream;
import java.io.PipedOutputStream;

import java.util.LinkedList;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.ListMemoizer;
import net.fortytwo.ripple.util.Sink;

public class QueryTest extends RippleTestCase
{
	private class ModelConstructorTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
		}
	}

	public void runTests()
		throws Throwable
	{
		testAsynchronous( new ModelConstructorTest() );
	}





	protected class ResultSet implements Sink<RippleList>
	{
		private ListMemoizer<RippleValue,String> memoizer = null;
		private String memo = "memo";

		public void put( final RippleList l )
			throws RippleException
		{
			if ( null == memoizer )
				memoizer = new ListMemoizer<RippleValue,String>( l, memo );
	
			else
				memoizer.add( l, memo );
		}

		public int compareTo( final ResultSet other )
		{
			if ( null == memoizer )
				return ( null == other.memoizer )
					? 0 : -1;
			else if ( null == other.memoizer )
				return 1;
			else
				return memoizer.compareTo( other.memoizer );
		}
	}







}

// kate: tab-width 4
