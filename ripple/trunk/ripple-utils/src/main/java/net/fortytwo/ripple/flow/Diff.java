package net.fortytwo.ripple.flow;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.flow.DiffSink;
import net.fortytwo.ripple.flow.Sink;

import java.util.LinkedList;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 1:39:10 PM
 */
// TODO: add a concept of Source
public class Diff<T, E extends Exception> implements DiffSink<T, E>
{
    private enum Action { Add, Remove };
    private class Change
    {
        public T value;
        public Action action;
    }

    // An order-preserving list of changes.
    private LinkedList<Change> changes;

    private Sink<T, E> plusSink;
    private Sink<T, E> minusSink;

    public Diff()
    {
        changes = new LinkedList<Change>();

        plusSink = new Sink<T, E>()
        {
            public void put(final T t) throws E
            {
                Change ch = new Change();
                ch.value = t;
                ch.action = Action.Add;
                changes.addLast(ch);
            }
        };

        minusSink = new Sink<T, E>()
        {
            public void put(final T t) throws E
            {
                Change ch = new Change();
                ch.value = t;
                ch.action = Action.Remove;
                changes.addLast(ch);
            }
        };
    }

    public Sink<T, E> getPlus()
    {
        return plusSink;
    }

    public Sink<T, E> getMinus()
    {
        return minusSink;
    }


    public void clear()
    {
        changes.clear();
    }

    public void writeTo(final DiffSink<T, E> sink) throws E
    {
        Sink<T, E> otherPlusSink = sink.getPlus();
        Sink<T, E> otherMinusSink = sink.getMinus();

        for (Change ch : changes) {
            switch (ch.action) {
                case Add:
                    otherPlusSink.put(ch.value);
                    break;
                case Remove:
                    otherMinusSink.put(ch.value);
                    break;
                default:
                    new RippleException("unsupported Action: " + ch.action).logError();
            }
        }
    }
}
