package net.fortytwo.ripple.io;

import java.util.Collection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RdfValue;

import org.openrdf.model.URI;

public interface Dereferencer
{
public void dereference( URI uri, ModelConnection mc )
	throws RippleException;

	public void dereference( RdfValue rv, ModelConnection mc )
		throws RippleException;

	public void forget( RdfValue rv, ModelConnection mc )
		throws RippleException;

	public void addSuccessMemo( String memo );
	public void addFailureMemo( String memo );

	public Collection<String> getSuccessMemos();
	public Collection<String> getFailureMemos();
}

// kate: tab-width 4
