CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

#DC Added:
#SRC_DIR = src
SRCS = ProduePlots.cxx EffPurTools.cxx  BreakdownTools.cxx DrawingTools.cxx #DrawingStyle.h
#SRCS = ProduePlots.cxx DrawingTools.cxx
#SRCS = ProduePlots.cxx #EffPurTools.cxx
OBJS = $(SRCS:.c=.o)

MAIN = ProduePlots

all:    $(MAIN)
	@echo  ProduePlots has compiled successfully.

#New main without minerva specific build:
$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN) $(OBJS)

clean:
	$(RM) *.o *~ $(MAIN)