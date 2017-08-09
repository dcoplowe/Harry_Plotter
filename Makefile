CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

SRCS =	$(wildcard app/App.cxx) \
		$(wildcard src/*.cxx) \
		$(wildcard src/parser/*.cxx)
	
		# $(wildcard src/base/*.cxx) \
		# $(wildcard src/breakdown/*.cxx) \
		$(wildcard src/effpur/*.cxx)  

OBJS = $(SRCS:.c=.o)

INCS =	-Isrc/ -Isrc/base -Isrc/parser -Isrc/breakdown -Isrc/effpur

MAIN = ProducePlots

all:    $(MAIN)
	@echo  ProducePlots has compiled successfully.

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(ROOTFLAGS)

.PHONY: clean
clean:
	$(RM) *.o *~ $(MAIN)

