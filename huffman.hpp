#ifndef __HUFFMAN_HPP__
#define __HUFFMAN_HPP__

#include <queue>
#include <vector>
#include <istream>
#include "trie.hpp"
#include "bit.hpp"

using std::priority_queue;
using std::vector;
using std::istream;

typedef vector<bit> code;

struct tnode {
	long long weight;
	trie<char> *tr;
};

class comparator {
public:
	bool operator() (const tnode &a, const tnode &b) const {
		return a.weight > b.weight;
	}
};

map<char, code> huffman (istream &in);

#endif