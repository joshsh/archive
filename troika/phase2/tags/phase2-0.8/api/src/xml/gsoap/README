Note: this gSOAP interface is out-of-date.  xmldom-gsoap.c should be made to
agree with ../xmldom.h if gSOAP's DOM parser is ever used again in this project,
and xmldom.h should be deleted.


Download :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Get the latest release of gSOAP from:
    http://www.cs.fsu.edu/~engelen/soap.html


Install ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

su

tar -xzf gsoap*.tar.gz
cd gsoap*

./configure
make
make install

# Put soapcpp2 and wsdl2h onto the path.
cd soapcpp2
ln -s soapcpp2 /usr/bin/soapcpp2
ln -s wsdl2h /usr/bin/wsdl2h


From the gSOAP README.txt ::::::::::::::::::::::::::::::::::::::::::::::::::::::

To build your projects, you need the executables:

soapcpp2/src/soapcpp2           gSOAP stub/skeleton compiler
soapcpp2/wsdl/wsdl2h            gSOAP WSDL parser

And you need the following libraries (build from stdsoap2.c[pp]):

soapcpp2/libgsoap++.a           C++ runtime
soapcpp2/libgsoapck++.a         C++ runtime with HTTP cookie support
soapcpp2/libgsoapssl++.a        C++ runtime with SSL (HTTPS) support
soapcpp2/libgsoap.a             C runtime
soapcpp2/libgsoapck.a           C runtime with HTTP cookie support
soapcpp2/libgsoapssl.a          C runtime with SSL (HTTPS) support


From gSOAP install output ::::::::::::::::::::::::::::::::::::::::::::::::::::::

| You can link your programs with -lgsoap++ for          |
| C++ projects created with soapcpp2 and you can link    |
| with -lgsoap for C projects generated with soapcpp2 -c |
|                                                        |
| There are also the corresponding libraries for cookie- |
| support (-lgsoapck and lgsoapck++)                     |
|                                                        |
| The include file stdsoap2.h has been installed.

