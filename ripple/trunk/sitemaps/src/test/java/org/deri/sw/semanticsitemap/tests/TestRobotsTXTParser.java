/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007-2008 Joshua Shinavier
 */


package org.deri.sw.semanticsitemap.tests;

import org.deri.sw.semanticsitemap.RobotsTXTParser;

public class TestRobotsTXTParser {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		RobotsTXTParser parser = new RobotsTXTParser("localhost");

		testFor(parser, "AllowMe/1.0");
		System.out.println("--");
		testFor(parser, "DenySecret/6.6.6");
		System.out.println("--");
		testFor(parser, "Nothing/0.0");
	}

	private static void testFor(RobotsTXTParser parser, String userAgent) {
		String[] files = { "/photos", "/secret", "/inkel", "/secret/visa.txt" };

		for (String file : files) {
			System.out.format("is %1$s allowed for %3$s: %2$s%n", file, parser
					.isRobotSafe(userAgent, file), userAgent);
		}
	}
}
