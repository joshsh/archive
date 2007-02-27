package wurfel.extensions.test;

import wurfel.Extension;
import wurfel.WurfelException;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.ModelConnection;

import java.net.URL;

public class TestExtension extends Extension
{
    protected Function[] getFunctions( ModelConnection mc )
        throws WurfelException
    {
        Function[] functions = {
            new BagElements( mc ),
            new IntegerAdd( mc ),
            new IntegerSubtract( mc ),
            new IntegerMultiply( mc ),
            new IntegerDivide( mc ),
            new IntegerMod( mc ),
            new ConcatenateStringsPrimitive( mc ),
            new ListElements( mc ),
            new Combinator_B( mc ),
            new Combinator_C( mc ),
            new Combinator_I( mc ),
            new Combinator_K( mc ),
            new Combinator_L( mc ),
            new Combinator_O( mc ),
            new Combinator_R( mc ),
            new Combinator_S( mc ),
            new Combinator_T( mc ),
            new Combinator_U( mc ),
            new Combinator_V( mc ),
            new Combinator_W( mc ),
            new Combinator_Y( mc ),
            new Combinator_w( mc ) };

        return functions;
    }

    protected URL[] getResources()
    {
/*
        URL[] urls = {
            this.getClass().getResource( "wurfel-test.rdf" ) };

        return urls;
*/
        return null;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
