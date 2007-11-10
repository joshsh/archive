mvn install

java -jar target/rippleserver-0.1-standalone.jar

wget http://localhost:8182/sink42

wget "http://localhost:8182/pipe?filter=http%3a//fortytwo.net/2007/08/ripple/math%23random&sink=http%3a//localhost%3a8182/rippleserver/sink42"
wget "http://localhost:8182/pipe?filter=http%3a//fortytwo.net/2007/08/ripple/math%23random&sink=http%3a//fortytwo.net/rippleserver/sink42"


wget http://localhost:8182/info/abc/def?param=123
