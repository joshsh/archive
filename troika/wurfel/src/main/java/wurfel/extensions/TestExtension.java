package wurfel.extensions;

import wurfel.Context;
import wurfel.Extension;
import wurfel.WurfelException;
import wurfel.model.Function;

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
            new wurfel.primitives.IntegerAdd( context ),
            new wurfel.primitives.IntegerSubtract( context ),
            new wurfel.primitives.IntegerMultiply( context ),
            new wurfel.primitives.IntegerDivide( context ),
            new wurfel.primitives.IntegerMod( context ),
            new wurfel.primitives.ConcatenateStringsPrimitive( context ),
            new wurfel.primitives.ListElements( context ),
            new wurfel.primitives.misc.Sha1SumOf( context ),
            new wurfel.primitives.misc.UriToString( context ),
            new wurfel.primitives.misc.UrlEncoding( context ),
            new wurfel.primitives.misc.UrlTarget( context ),
            new wurfel.primitives.misc.SwoogleIt( context ),
            new wurfel.primitives.misc.Grab( context ),
            new wurfel.primitives.misc.PingTheSemanticWeb( context ),
            new wurfel.combinators.Combinator_B( context ),
            new wurfel.combinators.Combinator_C( context ),
            new wurfel.combinators.Combinator_I( context ),
            new wurfel.combinators.Combinator_K( context ),
            new wurfel.combinators.Combinator_L( context ),
            new wurfel.combinators.Combinator_O( context ),
            new wurfel.combinators.Combinator_R( context ),
            new wurfel.combinators.Combinator_S( context ),
            new wurfel.combinators.Combinator_T( context ),
            new wurfel.combinators.Combinator_U( context ),
            new wurfel.combinators.Combinator_V( context ),
            new wurfel.combinators.Combinator_W( context ),
            new wurfel.combinators.Combinator_Y( context ),
            new wurfel.combinators.Combinator_w( context ) };

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
