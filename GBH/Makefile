OBJS = $(addsuffix .o,$(basename $(wildcard *.cpp)))
CXXFLAGS := $(CXXFLAGS) $(shell gosu-config --cxxflags)
LIBS := -lgosu -lGLEW $(shell gosu-config --libs)

BIN = GBH

$(BIN): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

#clean:
	#$(RM) $(BIN) $(OBJS)
