# Makefile template for shared library

ifeq ($(arch),X64) 
  ARCHITECTURE = -m64
  CC = g++-4.9
  CXX = g++-4.9
  LIBSPATH = libs/linux_x64/
  TARGET = libs/_tmpBuild/raumserver.so
endif
	
ifeq ($(arch),X86) 
  ARCHITECTURE = -m32 
  CC = g++-4.9
  CXX = g++-4.9
  LIBSPATH = libs/linux_x86/
  TARGET = libs/_tmpBuild/raumserver.so
endif

ifeq ($(arch),ARMA53)
  ARCHITECTURE = -march=armv8-a
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMA53/
  TARGET = libs/_tmpBuild/raumserver.so
endif

ifeq ($(arch),ARMV7HF)
  ARCHITECTURE = -march=armv7-a 
  CC = arm-linux-gnueabihf-g++-4.9 
  CXX = arm-linux-gnueabihf-g++-4.9
  LIBSPATH = libs/linux_ARMV7/
  TARGET = libs/_tmpBuild/raumserver.so
endif

ifeq ($(arch),ARMV6)
  ARCHITECTURE = -march=armv6
  CC = arm-linux-gnueabi-g++-4.9 
  CXX = arm-linux-gnueabi-g++-4.9
  LIBSPATH = libs/linux_ARMV6/
  TARGET = libs/_tmpBuild/raumserver.so
endif

ifeq ($(arch),ARMV5TE)
  ARCHITECTURE = -march=armv5te -mtune=xscale
  CC = arm-linux-gnueabi-g++-4.9 
  CXX = arm-linux-gnueabi-g++-4.9
  LIBSPATH = libs/linux_ARMV5/
  TARGET = libs/_tmpBuild/raumserver.so
endif


LIBSDEF =  $(LIBSPATH)raumkernel.so

DEBUG = 
ifeq ($(dbg),1)
  DEBUG = -g
endif

INCPATH = -I includes/ -I ../../RaumkernelLib/source/includes/

CFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -mapcs-frame -funwind-tables -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
CXXFLAGS = $(ARCHITECTURE) -std=c++11 -fPIC -Wall -Wno-unknown-pragmas -Wno-unused-parameter -mapcs-frame -funwind-tables -Wextra -O2 $(DEBUG) -c -pthread $(INCPATH)
LDFLAGS = $(ARCHITECTURE) -Wl,--no-as-needed -shared -pthread -static-libgcc -static-libstdc++ -rdynamic -ldl

RM = rm -f 

SRCS = $(wildcard *.cpp)
SRCS += $(wildcard manager/*.cpp)
SRCS += $(wildcard request/*.cpp)
SRCS += $(wildcard webserver/*.cpp)
SRCS += $(wildcard webserver/civetweb/*.cpp)
SRCS += $(wildcard webserver/civetweb/*.c)

OBJS = $(SRCS:.cpp=.o)


.PHONY: all
all: ${TARGET}

$(TARGET): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^ $(LIBSDEF)
 
$(SRCS:.cpp=.d):%.d:%.cpp
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.cpp=.d)


#.PHONY: clean
#clean:
#	-${RM} ${TARGET} ${OBJS} $(SRCS:.cpp=.d) 
.PHONY: clean
clean:
	-${RM} ${TARGET} ${OBJS} $(SRCS:.cpp=.d) 
