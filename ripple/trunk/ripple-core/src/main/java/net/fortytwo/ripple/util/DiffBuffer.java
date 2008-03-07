package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 3:07:52 PM
 */
public class DiffBuffer<T> extends Diff<T>
{
    private DiffSink<T> sink;

    public DiffBuffer( final DiffSink<T> sink )
    {
        this.sink = sink;
    }
    
    public void flush() throws RippleException
    {
        writeTo(sink);

        clear();
    }
}
