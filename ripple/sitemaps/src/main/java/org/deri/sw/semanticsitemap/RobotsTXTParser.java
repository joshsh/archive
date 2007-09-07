/**
 * 
 */
package org.deri.sw.semanticsitemap;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.List;

/**
 * Class that parses a site's robots.txt file according to <a
 * href="http://www.robotstxt.org/wc/exclusion.html#robotstxt">The Robots
 * Exclusion Protocol</a>.
 * 
 * @author Leandro M. López (inkel)
 * 
 */
public class RobotsTXTParser {

	/**
	 * Constant string that identifies the entry is for ALL robots.
	 */
	private static final String ALL_USER_AGENTS = "*";

	/**
	 * Constant string to identify Disallow sections.
	 */
	private static final String DISALLOW = "disallow";

	/**
	 * Constant string to identify User-Agent sections.
	 */
	private static final String USER_AGENT = "user-agent";

	/**
	 * 
	 */
	private Hashtable<String, List<String>> disallowedPrefix;

	/**
	 * Creates a robots.txt parser for host using HTTP protocol.
	 * 
	 * @param host
	 * @throws IOException
	 */
	public RobotsTXTParser(String host) throws IOException {
		this("http", host);
	}

	/**
	 * Creates a robots.txt parser for host using specified protocol.
	 * 
	 * @param protocol
	 * @param host
	 * @throws IOException
	 */
	public RobotsTXTParser(String protocol, String host) throws IOException {
		disallowedPrefix = new Hashtable<String, List<String>>();

		parseRobotsTXT(protocol, host);
	}

	private void parseRobotsTXT(String protocol, String host)
			throws IOException {
		try {
			URL robots = new URL(protocol, host, "/robots.txt");

			HttpURLConnection conn = (HttpURLConnection) robots
					.openConnection();
			conn.setInstanceFollowRedirects(false);
			conn.addRequestProperty("Host", host);

			BufferedReader br = new BufferedReader(new InputStreamReader(conn
					.getInputStream()));

			String line = null, userAgent = null;

			while (null != (line = br.readLine())) {
				int index = line.indexOf(':');

				if (-1 != index) {
					String section = line.substring(0, index).toLowerCase();
					String value = line.substring(index + 1).trim();

					if (USER_AGENT.equals(section)) {
						userAgent = value.toLowerCase();

						if (!disallowedPrefix.containsKey(userAgent)) {
							disallowedPrefix.put(userAgent,
									new ArrayList<String>());
						}
					} else if (null != userAgent && DISALLOW.equals(section)) {
						if (value.length() > 0) {
							disallowedPrefix.get(userAgent).add(value);
						} else {
							disallowedPrefix.remove(userAgent);
						}
					} else {
						userAgent = null;
					}
				}
			}
		} catch (MalformedURLException e) {
			// TODO What we should do in this case?
			e.printStackTrace();
		} catch (FileNotFoundException fnfex) {
			// No /robots.txt was found, so every file at host should be
			// available for retrieval
		}
	}

	/**
	 * Finds a matching key for the User-Agent
	 * 
	 * @param userAgent
	 *            Crawler's User-Agent header.
	 * @return The matching robots.txt User-Agent or null.
	 */
	private String foundKeyForUserAgent(String userAgent) {
		String key = null;

		userAgent = userAgent.toLowerCase();

		for (Enumeration<String> iter = disallowedPrefix.keys(); iter
				.hasMoreElements()
				&& (key == null);) {
			String k = iter.nextElement();
			if (userAgent.startsWith(k)) {
				key = k;
			}
		}

		return key;
	}

	/**
	 * Checks if the User-Agent is allowed to access the requested file.
	 * 
	 * @param userAgent
	 *            The crawler's <a
	 *            href="http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.43">User-Agent
	 *            header</a>.
	 * @param file
	 *            The file to be checked for allowed access.
	 * @return <code>true</code> if the crawler is allow to access the file.
	 */
	public boolean isRobotSafe(String userAgent, String file) {
		boolean allowed = true;

		String key;

		if (ALL_USER_AGENTS.equals(userAgent)) {
			key = ALL_USER_AGENTS;
		} else {
			key = foundKeyForUserAgent(userAgent);
		}

		if (null != key && disallowedPrefix.containsKey(key)) {
			for (String prefix : disallowedPrefix.get(key)) {
				if (file.startsWith(prefix)) {
					allowed = false;
					break;
				}
			}
		}

		if (allowed && !ALL_USER_AGENTS.equals(key)) {
			return isRobotSafe(ALL_USER_AGENTS, file);
		} else {
			return allowed;
		}
	}

	/**
	 * Checks if any User-Agent is allowed to access the requested file.
	 * 
	 * @param file
	 *            The file to be checked for allowed access.
	 * @return <code>true</code> if the crawler is allow to access the file.
	 */
	public boolean isRobotsSafe(String file) {
		return isRobotSafe(ALL_USER_AGENTS, file);
	}
}
