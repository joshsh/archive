################################################################################
## Makefile for the Phase2 importing utility.
##
## This program builds an object file which is linked with the Phase2 API source
## when an application is compiled.  To import a function [or data type], place a
## skeleton header file in the src directory.  If this function [or data type]
## depends on special libraries or compiler options, these must be specified in
## the project Makefile.

NAME         = p2-import

SRC_PATH     = ./src
XMLOUT_PATH  = ./xml
DOXYGEN_CFG  = $(NAME).cfg
DOXYGEN_OUT  = doxygen-out.xml
CC           = gcc


all : $(SRC_PATH)/$(NAME).o
	nm $(SRC_PATH)/$(NAME).o

clean :
	-rm $(DOXYGEN_OUT) -rf $(XMLOUT_PATH) $(HEADER) $(SRC_PATH)/$(NAME).*

$(SRC_PATH)/$(NAME).o : $(SRC_PATH)/$(NAME).c
	gcc -c -o $(SRC_PATH)/$(NAME).o $(SRC_PATH)/$(NAME).c

$(SRC_PATH)/$(NAME).c : $(DOXYGEN_OUT) header-generator.xslt
	xsltproc header-generator.xslt $(DOXYGEN_OUT) > $(SRC_PATH)/$(NAME).c

$(DOXYGEN_OUT) :  doxygen
	xsltproc $(XMLOUT_PATH)/combine.xslt $(XMLOUT_PATH)/index.xml > $(DOXYGEN_OUT)

.PHONY : doxygen
doxygen :
	doxygen $(DOXYGEN_CFG)

