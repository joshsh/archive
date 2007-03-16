package net.fortytwo.ripple.extensions.misc;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleStack;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.Value;
import org.openrdf.model.URI;
import org.openrdf.model.Literal;

import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Sha1SumOf extends PrimitiveFunction
{
    private static final String
        s_encoding = "UTF-8",
        s_algorithm = "SHA";

    private static MessageDigest messageDigest = null;

    public Sha1SumOf( ModelConnection mc )
        throws RippleException
    {
        super( mc.createRippleMiscUri( "sha1SumOf" ), mc );
    }

    // See: http://intertwingly.net/stories/2003/08/05/sha1demo.java
    private synchronized String encrypt( final String plaintext )
        throws RippleException
    {
        try
        {
            if ( null == messageDigest )
                messageDigest = MessageDigest.getInstance( s_algorithm );
        }

        catch( NoSuchAlgorithmException e )
        {
            throw new RippleException( e );
        }

        try
        {
            messageDigest.update( plaintext.getBytes( s_encoding ) );
        }

        catch( UnsupportedEncodingException e )
        {
            throw new RippleException( e );
        }

        try
        {
            byte digest[] = messageDigest.digest();

            String coded = "";

            for  ( byte b : digest )
            {
                String hex = Integer.toHexString( b );
                if ( hex.length() == 1 )
                    hex = "0" + hex;
                hex = hex.substring( hex.length() - 2 );
                coded += hex;
            }

            return coded;
        }

        catch ( Throwable t )
        {
            throw new RippleException( t );
        }
    }

    protected void applyInternal( RippleStack stack,
                                  Sink<RippleStack> sink,
                                  ModelConnection mc )
        throws RippleException
    {
        String a, result;

        a = mc.stringValue(
            mc.castToLiteral( stack.getFirst() ) );
        stack = stack.getRest();

        result = encrypt( a );

        sink.put( new RippleStack( mc.createLiteral( result ), stack ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
