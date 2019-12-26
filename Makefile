# Makefile for Cross Interleaved Reed Solomon encoder/decoder
#
# (c) Henry Minsky, Universal Access 1991-1996
#
# Makefile for Project for Wireless and Mobile Networks course on BUT FIT,
# adopted by Fridolin Pokorny
# 10 Oct 2013


RANLIB = ranlib
AR = ar

VERSION = 1.0.2
DIRNAME= rs-coder-$(VERSION)

CC = gcc
DEBUG_FLAGS = -g
CFLAGS = -Wall -Wstrict-prototypes  $(OPTIMIZE_FLAGS)
LDFLAGS = $(OPTIMIZE_FLAGS)

.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -Wextra

SRCS = common.cpp
HDRS = common.h
AUX  = Makefile
DOCS = LICENSE README.md

LIB_CSRC = rs.c galois.c berlekamp.c crcgen.c
LIB_HSRC = ecc.h
LIB_OBJS = rs.o galois.o berlekamp.o crcgen.o

TARGET_LIB = libecc.a
TARGET_LIBSO = libecc.so.1

TARGETS = $(TARGET_LIB) rsenc rsdec
TARGETS_SHARED = $(TARGET_LIBSO) rsenc-shared rsdec-shared

PACKNAME=$(DIRNAME).zip

all: $(TARGETS)

$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $^
	if [ "$(RANLIB)" ]; then $(RANLIB) $@; fi

rsenc: common.cpp $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -DBMS1A $(LDFLAGS) $^ -o $@

rsdec: common.cpp $(TARGET_LIB)
	$(CXX) $(CXXFLAGS) -DBMS1B $(LDFLAGS) $^ -o $@

shared: $(TARGETS_SHARED)

$(TARGET_LIBSO): $(LIB_OBJS)
	$(CC) -shared -Wl,-soname,$@ $(CFLAGS) $(LDFLAGS) $^ -o $@

rsenc-shared: common.cpp $(TARGET_LIBSO)
	$(CXX) $(CXXFLAGS) -DBMS1A $(LDFLAGS) $^ -o $@

rsdec-shared: common.cpp $(TARGET_LIBSO)
	$(CXX) $(CXXFLAGS) -DBMS1B $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o $(TARGETS) $(TARGETS_SHARED) ${PACKNAME}

dist:
	(cd ..; tar -cvf $(DIRNAME).tar $(DIRNAME))

depend:
	makedepend $(SRCS)

pack:
	zip -R ../$(PACKNAME) $(SRCS) $(HDRS) $(AUX) $(LIB_CSRC) $(LIB_HSRC) $(DOCS)


# DO NOT DELETE THIS LINE -- make depend depends on it.

