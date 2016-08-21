CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
INCLUDE += -I$(PLOTUTILSROOT)/ #-Isrc/
LcDLIBS += -L$(PLOTUTILSROOT)/$(CMTCONFIG) -lplotutils

#DC Added:
#SRC_DIR = src
#SRCS = MomentumDists.cxx EffPurTools.cxx DrawingTools.cxx
#SRCS = MomentumDists.cxx DrawingTools.cxx
SRCS = MomentumDists.cxx EffPurTools.cxx
OBJS = $(SRCS:.c=.o)

MAIN = MomentumDists

all:    $(MAIN)
	@echo  MomentumDists has compiled successfully.

$(MAIN): $(OBJS)
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN) $(OBJS) $(LDLIBS)

clean:
	$(RM) *.o *~ $(MAIN)

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
#:	rm -f $(wildcard *.o) $(patsubst %.cxx, %, $(wildcard *.cxx))
