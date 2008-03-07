package net.fortytwo.ripple.util;

import net.fortytwo.ripple.RippleException;

import java.util.LinkedList;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 1:39:10 PM
 */
// TODO: add a concept of Source
public class Diff<T> implements DiffSink<T>
{
    private enum Action { Add, Remove };
    private class Change
    {
        public T value;
        public Action action;
    }

    // An order-preserving list of changes.
    private LinkedList<Change> changes;

    private Sink<T> plusSink;
    private Sink<T> minusSink;

    public Diff()
    {
        changes = new LinkedList<Change>();

        plusSink = new Sink<T>()
        {
            public void put(final T t) throws RippleException
            {
                Change ch = new Change();
                ch.value = t;
                ch.action = Action.Add;
                changes.addLast(ch);
            }
        };

        minusSink = new Sink<T>()
        {
            public void put(final T t) throws RippleException
            {
                Change ch = new Change();
                ch.value = t;
                ch.action = Action.Remove;
                changes.addLast(ch);
            }
        };
    }

    public Sink<T> getPlus()
    {
        return plusSink;
    }

    public Sink<T> getMinus()
    {
        return minusSink;
    }


    public void clear()
    {
        changes.clear();
    }

    public void writeTo(final DiffSink<T> sink) throws RippleException
    {
        Sink<T> otherPlusSink = sink.getPlus();
        Sink<T> otherMinusSink = sink.getMinus();

        for (Change ch : changes) {
            switch (ch.action) {
                case Add:
                    otherPlusSink.put(ch.value);
                    break;
                case Remove:
                    otherMinusSink.put(ch.value);
                    break;
                default:
                    throw new RippleException("unsupported Action: " + ch.action);
            }
        }
    }
}
