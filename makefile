cpufeature = $(if $(findstring $(1),$(shell cat /proc/cpuinfo)),$(2))
PARAMS_SSE = $(call cpufeature,sse,-msse) $(call cpufeature,sse2,-msse2) $(call cpufeature,sse3,-msse3) $(call cpufeature,sse4a,-msse4a) $(call cpufeature,sse4_1,-msse4.1) $(call cpufeature,sse4_2,-msse4.2 -msse4) -mfpmath=sse 
PARAMS_NEON = -mfloat-abi=hard -march=armv6 -mtune=cortex-a8 -mfpu=neon -mvectorize-with-neon-quad -funsafe-math-optimizations -Wformat=0 -DNEON_OPTS
#tnx Jan Szumiec for the Raspberry Pi support
PARAMS_RASPI = -mfloat-abi=hard -mcpu=arm1176jzf-s -mfpu=vfp -funsafe-math-optimizations -Wformat=0
PARAMS_ARM = $(if $(call cpufeature,BCM2708,dummy-text),$(PARAMS_RASPI),$(PARAMS_NEON))
PARAMS_SIMD = $(if $(call cpufeature,sse,dummy-text),$(PARAMS_SSE),$(PARAMS_ARM))
PARAMS_LOOPVECT = -O3 -ffast-math -fdump-tree-vect-details -dumpbase dumpvect
#PARAMS_LIBS = -g -lm -lrt -lfftw3f -lstdc++ -DUSE_FFTW -DLIBCSDR_GPL -DUSE_IMA_ADPCM
PARAMS_LIBS = -g -lm -lstdc++ -lwiringPi
PARAMS_SO = -fpic  
PARAMS_MISC = -Wno-unused-result
#DEBUG_ON = 0 #debug is always on by now (anyway it could be compiled with `make DEBUG_ON=1`)
#PARAMS_DEBUG = $(if $(DEBUG_ON),-g,)
FFTW_PACKAGE = fftw-3.3.3

CC=gcc

CFLAGS= $(if $(shell uname -a | grep -i armv), -c -Wall -DLE_MACHINE -D_GNU_SOURCE $(PARAMS_LOOPVECT) $(PARAMS_SIMD) $(PARAMS_MISC), -c -Wall -DLE_MACHINE -D_GNU_SOURCE )
CXX = $(CC)
CXXFLAGS = $(CFLAGS) # set these flags for use of suffix rules for cc
LDFLAGS= $(PARAMS_LIBS)

SOURCES= tennisball.cc
OBJECTS=$(SOURCES:.cc=.o)

MODOBJ = FMMod.o

EXECUTABLE=tennisball

all: $(EXECUTABLE)

test:
	$(CC) $(CFLAGS) testIn1.cc -lm -o testIn1.o
	$(CC) $(LDFLAGS) testIn1.o -o testIn1 -lm 

$(EXECUTABLE): $(SOURCES) $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o tennisball

$(MODOBJ) : $(MODSRC)
	$(CC) $(CFLAGS) $*.cc -o $@

clean:
	rm -fr $(OBJECTS) $(EXECUTABLE) *.o
