mvn install
./run.sh


# Get a description of a sink
curl "http://localhost:8182/registered/sink1"

# Post some data into a sink
curl -d "ignore me" -H "Content-Type: application/rdf+xml" "http://localhost:8182/registered/debugSink" 




wget http://localhost:8182/sink42

wget "http://localhost:8182/pipe?filter=http%3a//fortytwo.net/2007/08/ripple/math%23random&sink=http%3a//localhost%3a8182/rippleserver/sink42"
wget "http://localhost:8182/pipe?filter=http%3a//fortytwo.net/2007/08/ripple/math%23random&sink=http%3a//fortytwo.net/rippleserver/sink42"


wget http://localhost:8182/info/abc/def?param=123
