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
using std::char_traits;

typedef vector<bit> code;

struct weighted_trie {
	long long weight;
	trie<int> *root;
};

class comparator {
public:
	bool operator() (const weighted_trie &a, const weighted_trie &b) const {
		return a.weight > b.weight;
	}
};

map<int, code> huffman (istream &in);

#endif