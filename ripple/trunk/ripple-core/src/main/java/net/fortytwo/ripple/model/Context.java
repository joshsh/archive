package net.fortytwo.ripple.model;

/**
 * Author: josh
 * Date: Jan 31, 2008
 * Time: 4:38:03 PM
 */
public class Context
{
	private ModelConnection modelConnection;

	public Context(final ModelConnection mc)
	{
		modelConnection = mc;
	}

	public ModelConnection getModelConnection()
	{
		return modelConnection;
	}
}
