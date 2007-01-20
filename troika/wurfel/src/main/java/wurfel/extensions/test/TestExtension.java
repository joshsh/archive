package wurfel.extensions.test;

import wurfel.Context;
import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;
import wurfel.model.EvaluationContext;

import java.net.URL;

public class TestExtension extends Extension
{
    public TestExtension( Context context )
    {
        super( context );
    }

    protected Function[] getFunctions()
        throws WurfelException
    {
        Function[] functions = {
            new IntegerAdd( context ),
            new IntegerSubtract( context ),
            new IntegerMultiply( context ),
            new IntegerDivide( context ),
            new IntegerMod( context ),
            new ConcatenateStringsPrimitive( context ),
            new ListElements( context ),
            new Combinator_B( context ),
            new Combinator_C( context ),
            new Combinator_I( context ),
            new Combinator_K( context ),
            new Combinator_L( context ),
            new Combinator_O( context ),
            new Combinator_R( context ),
            new Combinator_S( context ),
            new Combinator_T( context ),
            new Combinator_U( context ),
            new Combinator_V( context ),
            new Combinator_W( context ),
            new Combinator_Y( context ),
            new Combinator_w( context ) };

        return functions;
    }

    protected URL[] getResources()
    {
        URL[] urls = {
            this.getClass().getResource( "wurfel-test.rdf" ) };

        return urls;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
