package net.fortytwo.ripple.rdf.sail;

import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.test.RippleTestCase;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.UrlFactory;

import org.openrdf.model.vocabulary.RDF;

public class LinkedDataSailTest extends RippleTestCase
{
	private class DereferencerTest extends TestRunnable
	{
		public void test()
			throws Exception
		{
			Model model = getTestModel();
			UrlFactory urlFactory = getTestUrlFactory();
			
			// This is an example where HttpDereferencer fails by requesting the
			// full URI of a resource (rather than stripping off the local part).
			// Here, we define a mapping to a local file, so dereferencing
			// succeeds.
			urlFactory.addMapping( "http://www.holygoat.co.uk/owl/redwood/0.1/tags/Tagging",
					LinkedDataSailTest.class.getResource( "tags.owl" ).toString() );
			
			ModelConnection mc = model.getConnection( "" );
			RippleValue tagTagging = new RdfValue(
					mc.createUri( "http://www.holygoat.co.uk/owl/redwood/0.1/tags/Tagging" ) );
			RippleValue rdfType = new RdfValue( RDF.TYPE );
			
			Collector<RippleValue> results = new Collector<RippleValue>();
			
			mc.multiply( tagTagging, rdfType, results );
			assertEquals( 1, results.size() );
			
			mc.close();
		}
	}
	
	@Override
	public void runTests() throws Exception
	{
		this.testAsynchronous( new DereferencerTest() );
	}
}
