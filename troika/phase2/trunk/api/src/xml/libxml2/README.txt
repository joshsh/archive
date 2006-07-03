Download :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Get the latest version of libxml2 from:
     http://www.xmlsoft.org/downloads.html


Install ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

su

tar -xzf libxml*.tar.gz
cd libxml*

./configure
make
make install

# ldconfig

make tests


Compile an example program :::::::::::::::::::::::::::::::::::::::::::::::::::::

gcc -o example `xml2-config --cflags` example.c `xml2-config --libs`

