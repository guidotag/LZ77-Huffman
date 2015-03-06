CXX = g++
CXXFLAGS = -O2 -Wall -std=c++0x -pedantic

BIN = compress
OBJS = compressor.o lz77.o huffman.o bit.o

.PHONY: clean

$(BIN): $(OBJS)
	$(CXX) $^ -o $(BIN)

lz77.o: lz77.cpp lz77.hpp buffer.hpp trie.hpp
	$(CXX) -c $(CXXFLAGS) $<

huffman.o: huffman.cpp huffman.hpp trie.hpp
	$(CXX) -c $(CXXFLAGS) $<

compressor.o: compressor.cpp bit.hpp bit_writer.hpp bit_reader.hpp
	$(CXX) -c $(CXXFLAGS) $<

bit.o: bit.cpp bit.hpp
	$(CXX) -c $(CXXFLAGS) $<

clean:
	rm -f $(BIN)
	rm -f $(OBJS)