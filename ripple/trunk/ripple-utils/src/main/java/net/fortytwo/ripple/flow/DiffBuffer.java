package net.fortytwo.ripple.flow;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 3:07:52 PM
 */
public class DiffBuffer<T, E extends Exception> extends Diff<T, E>
{
    private DiffSink<T, E> sink;

    public DiffBuffer( final DiffSink<T, E> sink )
    {
        this.sink = sink;
    }
    
    public void flush() throws E
    {
        writeTo(sink);

        clear();
    }
}
