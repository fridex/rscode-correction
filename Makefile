# Makefile for Cross Interleaved Reed Solomon encoder/decoder
#
# (c) Henry Minsky, Universal Access 1991-1996
#
# Makefile for Project for Wireless and Mobile Networks course on BUT FIT,
# adopted by Fridolin Pokorny
# 10 Oct 2013


RANLIB = ranlib
AR = ar


VERSION = 1.0
DIRNAME= rscode-$(VERSION)


CC = gcc
# OPTIMIZE_FLAGS = -O69
DEBUG_FLAGS = -g
CFLAGS = -Wall -Wstrict-prototypes  $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS) -I..
LDFLAGS = $(OPTIMIZE_FLAGS) $(DEBUG_FLAGS)

.PHONY: clean

CXX = g++
CXXFLAGS = -Wall -g -Wextra

SRCS = common.cpp
HDRS = common.h
AUX  = Makefile

LIB_CSRC = rs.c galois.c berlekamp.c crcgen.c
LIB_HSRC = ecc.h
LIB_OBJS = rs.o galois.o berlekamp.o crcgen.o

TARGET_LIB = libecc.a
TEST_PROGS = example

TARGETS = $(TARGET_LIB) bms1A bms1B

PACKNAME=project.zip

all: $(TARGETS) bms1A bms1B

$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $(LIB_OBJS)
	if [ "$(RANLIB)" ]; then $(RANLIB) $@; fi

clean:
	rm -f *.o example libecc.a
	rm -f *~
	rm -f bms1A bms1B *.o ${PACKNAME}

dist:
	(cd ..; tar -cvf rscode-$(VERSION).tar $(DIRNAME))

depend:
	makedepend $(SRCS)

pack:
	zip -R $(PACKNAME) $(SRCS) $(HDRS) $(AUX) $(LIB_CSRC) $(LIB_HSRC)

bms1A: common.cpp $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ${LIB_OBJS} -DBMS1A common.cpp -o $@

bms1B: common.cpp $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ${LIB_OBJS} -DBMS1B common.cpp -o $@


# DO NOT DELETE THIS LINE -- make depend depends on it.

