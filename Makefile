CXX = g++
CXXFLAGS = -O2 -Wall -std=c++0x -pedantic

BIN = compress
OBJS = compressor.o lz77.o huffman.o

.PHONY: clean

$(BIN): $(OBJS)
	$(CXX) $^ -o $(BIN)

lz77.o: lz77.cpp buffer.hpp trie.hpp
	$(CXX) -c $(CXXFLAGS) $<

huffman.o: huffman.cpp trie.hpp
	$(CXX) -c $(CXXFLAGS) $<

compressor.o: compressor.cpp
	$(CXX) -c $(CXXFLAGS) $<

clean:
	rm -f $(BIN)
	rm -f $(OBJS)