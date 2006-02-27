########################################################
# Makefile for C++ programs including graphics library #
########################################################

CC	= /usr/local/gnu/bin/g++
INCLUDE	= -I${OPENWINHOME}/include -I.

#
# If you want to compile for debugging, change "-O" to "-g"
#

CFLAGS		= ${INCLUDE} -s

# 
# Libraries
#

LIBS	=  -L${OPENWINHOME}/lib -L/usr/local/gnu/lib\
           -lX11 -lg++ -lm

#
# make targets
#

%: %.C  graphics.h graphics.o
	${CC} ${CFLAGS} -o $* $*.C graphics.o ${LIBS} 

graphics.o: graphics.c graphics.h 
	${CC} ${CFLAGS} -c -o graphics.o graphics.c
 


	 

