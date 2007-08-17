================================================================================
Overview
================================================================================

Ripple is dedicated scripting language for the Semantic Web.  Its programs both
operate upon and reside in RDF graphs.  Ripple is a variation on the
"concatenative" theme of functional, stack-oriented languages such as Joy and
Factor, and takes a multivalued, pipeline approach to query composition.  This
Java implementation includes a query API, an extensible library of primitive
functions, and an interactive command-line interpreter.

For more information, including documentation and code samples, see:

    http://ripple.fortytwo.net/


================================================================================
Run
================================================================================

To begin an interpreter session, use one of the shortcut scripts (ripple or
ripple.bat) or invoke Ripple's Java archive directly:

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

To build Ripple from scratch, install Maven 2.0.x and issue the command

    $ mvn install

from the directory containing pom.xml (note: requires Java 5).  To build
Ripple with a custom configuration, modify the properties file at...

    ./src/main/resources/net/fortytwo/ripple/ripple.properties

...and recompile.


================================================================================
Bugs
================================================================================

Please submit bug reports to the tracker at:

    http://projects.semwebcentral.org/tracker/?atid=573&group_id=125

or send mail to josh@fortytwo.net.
