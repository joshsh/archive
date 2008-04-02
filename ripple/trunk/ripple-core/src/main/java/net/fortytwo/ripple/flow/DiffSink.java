package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.flow.Sink;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 1:20:34 PM
 */
public interface DiffSink<T, E extends Exception>
{
    Sink<T, E> getPlus();
    Sink<T, E> getMinus();
}
