CC=gcc

CFLAGS= -c -Wall -march=armv6 
LDFLAGS= -g -lm -lstdc++ -lwiringPi -pthread

CXX = $(CC)
CXXFLAGS = $(CFLAGS)

SOURCES= tennisball.cc
OBJECTS=$(SOURCES:.cc=.o)

EXECUTABLE=tennisball

all: $(EXECUTABLE)


$(EXECUTABLE): $(SOURCES) $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o tennisball

clean:
	rm -fr $(OBJECTS) $(EXECUTABLE) *.o
