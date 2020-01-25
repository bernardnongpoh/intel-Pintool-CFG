#===========================================================================
#
# File Name    : Makefile
# Description  : Makefile for BUBBLE
# Release Date : 24/11/2014
# Author       : PolYU
#
# Note         : Edit SYSTEMC_HOME path
# Revision History
#---------------------------------------------------------------------------
# Date      Version    Author      Description
#---------------------------------------------------------------------------
#24/11/2014      1.0    PolyU       Makefile for BUBBLE
#===========================================================================


TARGET	= bubble.exe

SRCS	=  \
	main.cpp \
	tb_bubble.cpp \
	bubble.cpp

HDRS	=  \
	bubble.h \
	tb_bubble.h \
	define.h

CYBER_SYSTEMC_HOME = /usr/local/systemc-2.3.3

ifneq (, $(wildcard /bin/uname))
CPU_ARCH = $(shell /bin/uname -m)
endif

ifeq ($(CPU_ARCH),x86_64)
CWB_LIB   = $(CYBER_PATH)/lib64
ARCH_FLAG = 
else
CWB_LIB   = $(CYBER_PATH)/lib
ARCH_FLAG = -m32
endif

ifdef CYBER_SYSTEMC_TARGET_ARCH
CWB_SC_ARCH = $(CYBER_SYSTEMC_TARGET_ARCH)
else
ifeq (, $(findstring Windows,$(OS)))
ifeq ($(CPU_ARCH),x86_64)
CWB_SC_ARCH = linux64
else
CWB_SC_ARCH = linux
endif
else
CWB_SC_ARCH = mingw
endif
endif

#ifdef CYBER_SYSTEMC_HOME
CWB_SC_HOME = $(CYBER_SYSTEMC_HOME)
#else
#CWB_SC_HOME = $(CYBER_PATH)/osci
#endif

CWB_SC_INCL = -I"$(CWB_SC_HOME)/include"
#CWB_SC_LIB  = "$(CWB_SC_HOME)/lib-$(CWB_SC_ARCH)/libsystemc.a"
#CWB_SC_LIB  = "$(CWB_SC_HOME)_CXX98/lib/libsystemc.a"
CWB_SC_LIB  = "$(CWB_SC_HOME)/lib-linux64/libsystemc.a"

EMPTY	=
SPACE	= $(EMPTY) $(EMPTY)
LINKER	= $(CC)
#LINKER=afl-clang++
COVER   = -fprofile-arcs -ftest-coverage 
#LINKER	= $(CC)
#LINKER	= afl-clang-fast
INCL	= -I"."
CFLAGS	= $(ARCH_FLAG) -O1
LDFLAGS	= $(ARCH_FLAG)
LIBS	= -lm $(CWB_SC_LIB)  -Wl,-rpath="$(CWB_LIB)" $(COVER)
OBJS	= $(notdir $(addsuffix .o,$(basename $(SRCS))))
debug: CFLAGS += -g -DDEBUG
wave: CFLAGS += -DWAVE_DUMP
swm: CFLAGS += -DSWM
cwom:CFLAGS += -DCWOM
cwom_tri:CFLAGS += -DCWOM_TRI
swm_tri: CFLAGS += -DSWM_TRI

$(TARGET) : $(OBJS)
	$(LINKER) -o "$@" $(LDFLAGS) $(OBJS) $(LIBS)

debug: $(OBJS)
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(DEBUG)

wave:  $(OBJS) 
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(WAVEFLAG)

swm:  $(OBJS) 
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(SWMFLAG)

cwom:  $(OBJS) 
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(CWOMFLAG)

cwom_tri:  $(OBJS) 
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(CWOM_TRIFLAG)

swm_tri:  $(OBJS) 
	$(LINKER) -o $(TARGET) $(LDFLAGS) $(OBJS) $(LIBS) $(SWM_TRIFLAG)

main.o: main.cpp
	$(CC) $(CFLAGS) $(CWB_SC_INCL) $(INCL) -c $< -o $@
tb_bubble.o: tb_bubble.cpp
	$(CC) $(CFLAGS) $(CWB_SC_INCL) $(INCL) -c $< -o $@
bubble.o: bubble.cpp
	$(CC) $(CFLAGS) $(CWB_SC_INCL) $(INCL) -c $< -o $@ -fprofile-arcs -ftest-coverage

clean :
	@$(RM) -f $(OBJS) $(TARGET)

depend :
	makedepend -- $(INCL) $(CFLAGS) -- -Y -fMakefile.GNU $(SRCS)

