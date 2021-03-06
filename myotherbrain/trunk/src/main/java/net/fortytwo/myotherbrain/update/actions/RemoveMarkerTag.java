package net.fortytwo.myotherbrain.update.actions;

import net.fortytwo.myotherbrain.model.concepts.FirstClassItem;
import net.fortytwo.myotherbrain.model.concepts.Marker;
import net.fortytwo.myotherbrain.update.WriteAction;
import net.fortytwo.myotherbrain.update.WriteContext;
import net.fortytwo.myotherbrain.update.UpdateException;

import java.net.URI;
import java.util.Set;

/**
 * Author: josh
 * Date: Jun 28, 2009
 * Time: 12:03:59 AM
 */
public class RemoveMarkerTag extends WriteAction {
    private final URI targetMarkerTag;

    private Set<URI> beforeMarkerTag;

    public RemoveMarkerTag(URI subject,
                           URI targetMarkerTag,
                           final WriteContext c) throws UpdateException {
        super(subject, c);

        if (null == targetMarkerTag) {
            throw new NullPointerException();
        } else {
            targetMarkerTag = c.normalizeResourceURI(targetMarkerTag);
        }

        this.targetMarkerTag = targetMarkerTag;
    }

    protected void executeUndo(final WriteContext c) throws UpdateException {
        FirstClassItem subject = this.toThing(this.subject, FirstClassItem.class, c);
        subject.setMarkerTag(toThingSet(beforeMarkerTag, Marker.class, c));
    }

    protected void executeRedo(final WriteContext c) throws UpdateException {
        FirstClassItem subject = this.toThing(this.subject, FirstClassItem.class, c);
        Set<Marker> markerTag = subject.getMarkerTag();
        beforeMarkerTag = toURISet(markerTag);

        // Note: assumes that the returned Set is modifiable.
        markerTag.remove(toThing(targetMarkerTag, Marker.class, c));
        // Note: assumes that removal from the Set is not necessarily persisted.
        subject.setMarkerTag(markerTag);
    }
}
