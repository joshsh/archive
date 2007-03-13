package net.fortytwo.ripple.cli.ast;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Function;
import net.fortytwo.ripple.model.FunctionEnvelope;
import net.fortytwo.ripple.model.ListNode;

import org.openrdf.model.URI;
import org.openrdf.model.Value;

public class ApplyNode extends Ast
{
    private Ast arg;

    public ApplyNode( final Ast argument )
    {
        arg = argument;
    }

    public Value evaluate( Interpreter itp, ModelConnection mc )
        throws RippleException
    {
        Value v = mc.getModel().translateFromGraph( arg.evaluate( itp, mc ) );

        return FunctionEnvelope.createEnvelope( v );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
