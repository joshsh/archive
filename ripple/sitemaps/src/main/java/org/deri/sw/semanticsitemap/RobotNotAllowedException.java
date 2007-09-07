package org.deri.sw.semanticsitemap;

import java.net.URL;

/**
 * Used to report that the {@link URL} is not allowed to be accessed by a robot.
 * 
 * @author Leandro M. López (inkel)
 * 
 */
public class RobotNotAllowedException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 2742163861317673882L;

	/**
	 * Constructs a <code>RobotNotAllowedException</code> for the specified
	 * {@link URL}.
	 * 
	 * @param url
	 */
	public RobotNotAllowedException(URL url) {
		super("Robots are not allowed to access URL: " + url);
	}

}
