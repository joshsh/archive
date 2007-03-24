package net.fortytwo.ripple.cli.ast;

import java.util.Iterator;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.Operator;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;

public class OperatorAst implements Ast
{
    private Ast arg;

    public OperatorAst( final Ast argument )
    {
        arg = argument;
    }

    public void evaluate( Sink<RippleValue> sink,
                          Interpreter itp,
                          ModelConnection mc )
        throws RippleException
    {
        ContainerSink values = new ContainerSink();
        arg.evaluate( values, itp, mc );
        for ( Iterator<RippleValue> iter = values.iterator(); iter.hasNext(); )
            sink.put( Operator.createOperator( iter.next(), mc ) );
    }

    public String toString()
    {
        return "/" + arg;
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
