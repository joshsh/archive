package net.fortytwo.myotherbrain.writeapi.actions;

import net.fortytwo.myotherbrain.MOBModelConnection;

import java.net.URI;

/**
 * Author: josh
 * Date: Jun 28, 2009
 * Time: 12:03:59 AM
 */
public class AddMarkerTag extends WriteAction {
    private final URI subject;
    private final URI tag;

    public AddMarkerTag(final URI subject,
                        final URI tag) {
        if (null == subject) {
            throw new NullPointerException();
        }

        if (null == tag) {
            throw new NullPointerException();
        }

        this.subject = subject;
        this.tag = tag;
    }

    protected void executeUndo(final MOBModelConnection c) throws NoSuchItemException {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    protected void executeRedo(final MOBModelConnection c) throws NoSuchItemException {
        //To change body of implemented methods use File | Settings | File Templates.
    }
}
