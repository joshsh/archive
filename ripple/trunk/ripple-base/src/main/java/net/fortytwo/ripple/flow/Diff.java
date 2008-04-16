/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package net.fortytwo.ripple.flow;

import java.util.LinkedList;

import org.apache.log4j.Logger;
import net.fortytwo.ripple.RippleException;

/**
 * Author: josh
 * Date: Mar 7, 2008
 * Time: 1:39:10 PM
 */
// TODO: add a concept of Source
public class Diff<T, E extends Exception> implements DiffSink<T, E>
{
    private static final Logger LOGGER = Logger.getLogger(Diff.class);

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
                                                   // throws E
    public void writeTo(final DiffSink<T, E> sink) throws Exception
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
                    //new RippleException("e").logError();
                    LOGGER.error("unsupported Action: " + ch.action);
            }
        }
    }
}
