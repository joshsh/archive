package wurfel.extensions.misc;

import wurfel.Wurfel;
import wurfel.WurfelException;
import wurfel.Context;
import wurfel.model.PrimitiveFunction;
import wurfel.model.NodeSet;
import wurfel.model.EvaluationContext;

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

    public Sha1SumOf( EvaluationContext evalContext )
        throws WurfelException
    {
        super( Wurfel.createUri( "urn:net.dnsdojo.troika.wurfel-misc#sha1SumOf" ), evalContext );
    }

    // See: http://intertwingly.net/stories/2003/08/05/sha1demo.java
    private synchronized String encrypt( final String plaintext )
        throws WurfelException
    {
        try
        {
            if ( null == messageDigest )
                messageDigest = MessageDigest.getInstance( s_algorithm );
        }

        catch( NoSuchAlgorithmException e )
        {
            throw new WurfelException( e );
        }

        try
        {
            messageDigest.update( plaintext.getBytes( s_encoding ) );
        }

        catch( UnsupportedEncodingException e )
        {
            throw new WurfelException( e );
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
            throw new WurfelException( t );
        }
    }

    protected Collection<Value> applyInternal( LinkedList<Value> args,
                                               EvaluationContext evalContext )
        throws WurfelException
    {
        String a, result;

        Iterator<Value> argIter = args.iterator();
        a = evalContext.stringValue(
                evalContext.castToLiteral( argIter.next() ) );

        result = encrypt( a );

        return new NodeSet( evalContext.createLiteral( result ) );
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on
