CXX = g++
CXXFLAGS = -std=c++11 -Wall
SOURCES = main.cpp
HPPS = studentRecord.hpp Tree/internalNode.hpp Tree/leafNode.hpp Tree/node.hpp Tree/tree.hpp
EXEC = project2

all: $(SOURCES) $(HPPS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -I./Tree/ -I. -o $(EXEC)

clean:
	rm project2*
