package wurfel.extensions.test;

import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;
import wurfel.model.EvaluationContext;

import java.net.URL;

public class TestExtension extends Extension
{
    protected Function[] getFunctions( EvaluationContext evalContext )
        throws WurfelException
    {
        Function[] functions = {
            new IntegerAdd( evalContext ),
            new IntegerSubtract( evalContext ),
            new IntegerMultiply( evalContext ),
            new IntegerDivide( evalContext ),
            new IntegerMod( evalContext ),
            new ConcatenateStringsPrimitive( evalContext ),
            new ListElements( evalContext ),
            new Combinator_B( evalContext ),
            new Combinator_C( evalContext ),
            new Combinator_I( evalContext ),
            new Combinator_K( evalContext ),
            new Combinator_L( evalContext ),
            new Combinator_O( evalContext ),
            new Combinator_R( evalContext ),
            new Combinator_S( evalContext ),
            new Combinator_T( evalContext ),
            new Combinator_U( evalContext ),
            new Combinator_V( evalContext ),
            new Combinator_W( evalContext ),
            new Combinator_Y( evalContext ),
            new Combinator_w( evalContext ) };

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
