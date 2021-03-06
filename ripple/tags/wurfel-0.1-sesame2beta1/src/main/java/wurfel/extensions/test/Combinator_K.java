package wurfel.extensions.test;

import wurfel.WurfelException;
import wurfel.model.Combinator;
import wurfel.model.Container;
import wurfel.model.ModelConnection;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_K extends Combinator
{
    public Combinator_K( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "K" ), mc );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      ModelConnection mc )
        throws WurfelException
    {
        Value x;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
//        Value y = argIter.next();

        return new Container( x );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
