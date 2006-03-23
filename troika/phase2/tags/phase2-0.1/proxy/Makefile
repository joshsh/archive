PARENT_PATH = ../..
GSOAP_PATH = $(PARENT_PATH)/gsoap-linux-2.7

GSOAP = $(GSOAP_PATH)/soapcpp2
SOAPH = $(GSOAP_PATH)/stdsoap2.h
SOAPC = $(GSOAP_PATH)/stdsoap2.c
SOAPCPP = $(GSOAP_PATH)/stdsoap2.cpp
CC=gcc
CPP=g++
LIBS=
COFLAGS=-O2
CWFLAGS=-Wall
CIFLAGS=-I../..
CMFLAGS=
CFLAGS= $(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS)
all:		gSOAP_proxy
gSOAP_proxy:	gSOAP_proxy.h gSOAP_proxy.c $(SOAPH) $(SOAPC)
		$(GSOAP) -c gSOAP_proxy.h
		$(CC) $(CFLAGS) -o gSOAP_proxy gSOAP_proxy.c soapC.c soapClient.c $(SOAPC) $(LIBS)
clean:
		rm -f *.o soapH.h soapStub.h soapC.cpp soapC.c soapClient.cpp soapClient.c soapServer.cpp soapServer.c soap*Proxy.h
distclean:
		rm -f *.o *.wsdl *.xsd *.xml *.nsmap *.log soapH.h soapStub.h soapC.cpp soapC.c soapClient.cpp soapClient.c soapServer.cpp soapServer.c soap*Proxy.h gSOAP_proxy
