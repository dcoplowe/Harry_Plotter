CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
INCLUDE += -I$(PLOTUTILSROOT)/
LDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG) -lplotutils

#DC Added:
SRCES := $(wildcard src/*.h)
DEPS=$(SRCES)

all : $(patsubst %.cxx, %.o, $(wildcard *.cxx))

%.o : %.cxx
    $(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o $(LDLIBS) -c $*.cxx
    $(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) $(LDLIBS) -o $* $*.o

# make a binary for every .cxx file
#all : $(patsubst %.cxx, %.o, $(wildcard *.cxx))

# rule for each one
#%.o : %.cxx
#	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o $(LDLIBS) -c $*.cxx #compile
#	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) $(LDLIBS) -o $* $*.o        #link

#clean:
#	rm -f $(wildcard *.o) $(patsubst %.cxx, %, $(wildcard *.cxx))
