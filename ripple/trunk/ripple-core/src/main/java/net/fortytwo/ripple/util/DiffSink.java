package net.fortytwo.ripple.util;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 1:20:34 PM
 */
public interface DiffSink<T>
{
    Sink<T> getPlus();
    Sink<T> getMinus();
}
