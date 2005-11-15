################################################################################
## Makefile for the Phase2 importing utility.
##
## [This program builds a header file which is included in the Phase2 API source
## when an application is compiled.  To import a function or data type, place a
## skeleton in stubs.h in the src directory.  If this function or data type
## depends on special libraries or compiler options, these must be worked into
## the project Makefile by hand.]

NAME         = p2-import

SRC_PATH     = ./src
XMLOUT_PATH  = ./xml
CFG_FILE     = $(NAME).cfg
DOXYGEN_OUT  = doxygen-out.xml
HEADER       = p2-import-header.h

all : $(HEADER)

clean :
	-rm $(DOXYGEN_OUT) -rf $(XMLOUT_PATH) $(HEADER)

$(HEADER) : $(DOXYGEN_OUT) header-generator.xslt
	xsltproc header-generator.xslt $(DOXYGEN_OUT) > $(HEADER)

$(DOXYGEN_OUT) :  doxygen
	xsltproc $(XMLOUT_PATH)/combine.xslt $(XMLOUT_PATH)/index.xml > $(DOXYGEN_OUT)

.PHONY : doxygen
doxygen :
	doxygen $(CFG_FILE)

