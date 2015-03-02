CXX = g++
CXXFLAGS = -O2 -Wall -std=c++0x -pedantic

.PHONY: clean

lz77: lz77.o 
	$(CXX) lz77.o -o lz77

lz77.o: lz77.cpp buffer.hpp trie.hpp
	$(CXX) -c $(CXXFLAGS) $<

clean:
	rm -f lz77
	rm -f lz77.o