CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

#DC Added:
#SRC_DIR = src
SRCS = ProducePlots.cxx EffPurTools.cxx BreakdownTools.cxx DataClasses.cxx DrawingTools.cxx
OBJS = $(SRCS:.c=.o)

MAIN = ProducePlots

all:    $(MAIN)
	@echo  ProducePlots has compiled successfully.

#New main without minerva specific build:
$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN) $(OBJS)

clean:
	$(RM) *.o *~ $(MAIN)