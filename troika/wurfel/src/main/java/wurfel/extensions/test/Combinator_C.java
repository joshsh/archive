package wurfel.extensions.test;

import wurfel.WurfelException;
import net.fortytwo.ripple.model.Apply;
import net.fortytwo.ripple.model.Combinator;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

public class Combinator_C extends Combinator
{
    public Combinator_C( ModelConnection mc )
        throws WurfelException
    {
        super( mc.createWurfelTestUri( "C" ), mc );
    }

    public Collection<Value> applyTo( LinkedList<Value> args,
                                      ModelConnection mc )
        throws WurfelException
    {
        Value x, y, z;

        Iterator<Value> argIter = args.iterator();
        x = argIter.next();
        y = argIter.next();
        z = argIter.next();

        Value result = new Apply(
            new Apply( x, z ),
            y );

        return new Container( result );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
