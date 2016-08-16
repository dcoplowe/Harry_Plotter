CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
INCLUDE += -I$(PLOTUTILSROOT)/ #-Isrc/
LDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG) -lplotutils

#DC Added:
#SRC_DIR = src
SRCS = MomentumDists.cxx EffPurTools.cxx
OBJS = $(SRCS:.c=.o)

MAIN = MomentumDists

all:    $(MAIN)
	@echo  Simple compiler named MomentumDists has been compiled

$(MAIN): $(OBJS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -o $(MAIN) $(OBJS) $(LDLIBS)

#all: MomentumDists

#MomentumDists: MomentumDists.o EffPurTools.o
#	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o MomentumDists MomentumDists.o $(LDLIBS) -c MomenumtDists.cxx 


# make a binary for every .cxx file
#all : $(patsubst %.cxx, %.o, $(wildcard *.cxx))
# rule for each one
#%.o : %.cxx
#	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o $(LDLIBS) -c $*.cxx #compile
#	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) $(LDLIBS) -o $* $*.o        #link
#
#clean:
#	rm -f $(wildcard *.o) $(patsubst %.cxx, %, $(wildcard *.cxx))
