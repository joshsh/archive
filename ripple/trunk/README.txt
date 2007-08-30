================================================================================
Overview
================================================================================

Ripple is dedicated scripting language for the Semantic Web.  Ripple programs
not only query the Semantic Web, but also reside within it as RDF data
structures, forming a global network of interlinked programs.  Ripple is a
relational stack language, closely related to functional stack languages such as
Joy, Factor and Cat.  As a Semantic Web interface, Ripple is a fast, text-based
linked data crawler and browser with all of the flexibility of a Turing-complete
programming language.

This Java implementation includes a query API, an extensible library of primitive
functions, and an interactive command-line interpreter.

For more information, including documentation and code samples, see:

    http://ripple.fortytwo.net/


================================================================================
Run
================================================================================

To begin an interpreter session, use one of the shortcut scripts (ripple or
ripple.bat):

    $ ./ripple

or invoke the executable JAR directly:

    $ java -jar target/ripple-*-standalone.jar

By default, Ripple uses a TriG cache file to save state between sessions.  To
create a new cache, use the @saveas directive while talking to the interpreter:

    >>  @saveas "cache.trig".

To restore a previous session, specify the cache as an argument at the command
line:

    $ ./ripple cache.trig

When you quit the application, the triple store is written back to the cache.

    >>  @quit.


================================================================================
Build
================================================================================

To build Ripple from scratch, issue the command:

    $ mvn install

from the directory containing pom.xml (note: requires Maven 2 and Java 5).  To
customize Ripple, modify this file:

    ./src/main/resources/net/fortytwo/ripple/ripple.properties

...then recompile.


================================================================================
Bugs
================================================================================

Please submit bug reports to the tracker at:

    http://projects.semwebcentral.org/tracker/?atid=573&group_id=125

or send mail to josh@fortytwo.net.
