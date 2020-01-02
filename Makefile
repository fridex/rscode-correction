# Makefile for Cross Interleaved Reed Solomon encoder/decoder
#
# (c) Henry Minsky, Universal Access 1991-1996
#
# Makefile for Project for Wireless and Mobile Networks course on BUT FIT,
# adopted by Fridolin Pokorny
# 10 Oct 2013


RANLIB = ranlib
AR = ar

SOVERSION = 1
VERSION = $(SOVERSION).1.0
DIRNAME= rs-coder-$(VERSION)

CC = gcc
DEBUG_FLAGS = -g
CFLAGS = -Wall -Wstrict-prototypes  $(OPTIMIZE_FLAGS)
LDFLAGS = $(OPTIMIZE_FLAGS)

.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -Wextra

SRCP = src
SRCL = $(SRCP)/lib
SRCS = $(SRCP)/coder.cpp
HDRS = $(SRCP)/coder.h
AUX  = Makefile
DOCS = LICENSE README.md

LIB_CSRC = $(SRCL)/rs.c $(SRCL)/galois.c $(SRCL)/berlekamp.c $(SRCL)/crcgen.c
LIB_HSRC = $(SRCL)/ecc.h
LIB_OBJS = $(SRCL)/rs.o $(SRCL)/galois.o $(SRCL)/berlekamp.o $(SRCL)/crcgen.o

TARGET_LIB = libecc.a
TARGET_LIBSO = libecc.so.$(SOVERSION)

TARGETS = $(TARGET_LIB) rsenc rsdec
TARGETS_SHARED = $(TARGET_LIBSO) rsenc-shared rsdec-shared

PACKNAME=$(DIRNAME).zip

all: $(TARGETS)

$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $^
	if [ "$(RANLIB)" ]; then $(RANLIB) $@; fi

rsenc: $(SRCS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -DBMS1A $(LDFLAGS) $^ -o $@

rsdec: $(SRCS) $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -DBMS1B $(LDFLAGS) $^ -o $@

shared: $(TARGETS_SHARED)

$(TARGET_LIBSO): $(LIB_OBJS)
	$(CC) -shared -Wl,-soname,$@ $(CFLAGS) $(LDFLAGS) $^ -o $@

rsenc-shared: $(SRCS) $(TARGET_LIBSO)
	$(CXX) $(CXXFLAGS) -DBMS1A $(LDFLAGS) $^ -o $@

rsdec-shared: $(SRCS) $(TARGET_LIBSO)
	$(CXX) $(CXXFLAGS) -DBMS1B $(LDFLAGS) $^ -o $@

clean:
	rm -f $(SRCL)/*.o $(TARGETS) $(TARGETS_SHARED) ${PACKNAME}

dist:
	(cd ..; tar -cvf $(DIRNAME).tar $(DIRNAME))

depend:
	makedepend $(SRCS)

pack:
	zip -R ../$(PACKNAME) $(SRCS) $(HDRS) $(AUX) $(LIB_CSRC) $(LIB_HSRC) $(DOCS)


# DO NOT DELETE THIS LINE -- make depend depends on it.

