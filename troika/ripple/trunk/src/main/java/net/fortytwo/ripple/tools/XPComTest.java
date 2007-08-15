/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.tools;

public class XPComTest {
	static public String getGreetings() {
		return "Hello World!";
	}

	private int m_count;

	public XPComTest(int start) {
		m_count = start;
	}

	public void count() {
		m_count++;
	}

	public int getCount() {
		return m_count;
	}
}

// kate: tab-width 4
