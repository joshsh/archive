package net.fortytwo.ripple.util;

public class Pointer<T>
{
	public T ref;

	public Pointer()
	{
		ref = null;
	}

	public Pointer( final T ref )
	{
		this.ref = ref;
	}
}

// kate: tab-width 4
