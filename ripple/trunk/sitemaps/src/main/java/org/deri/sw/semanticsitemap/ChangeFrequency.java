package org.deri.sw.semanticsitemap;

/*
 * http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd
 * 
 * OPTIONAL: Indicates how frequently the content at a particular URL is
 likely to change. The value "ALWAYS" should be used to describe
 documents that change each time they are accessed. The value "never"
 should be used to describe archived URLs. Please note that web
 crawlers may not necessarily crawl pages marked "ALWAYS" more often.
 Consider this element as a friendly suggestion and not a command.
 */

/**
 * Enumeration that indicates how often a resource is updated.
 * 
 * @author Leandro M. López (inkel)
 * 
 */
public enum ChangeFrequency {
	/**
	 * Content is changed every time it's accessed.
	 */
	ALWAYS,
	/**
	 * Content is changed once a day.
	 */
	DAILY,
	/**
	 * Content is changed every hour.
	 */
	HOURLY,
	/**
	 * Content is changed once a month.
	 */
	MONTHLY,
	/**
	 * Content won't change anymore. Used by archived resources.
	 */
	NEVER,
	/**
	 * Content is changed once a week.
	 */
	WEEKLY,
	/**
	 * Content is changed every year.
	 */
	YEARLY;
}
