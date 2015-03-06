#ifndef __LZ77_HPP__
#define __LZ77_HPP__

#include <iostream>
#include <list>
#include <map>
#include "trie.hpp"
#include "buffer.hpp"

using std::istream;
using std::ostream;
using std::list;
using std::map;
using std::max;

struct token {
	int distance;
	int length;
	char character;
};

void encode (istream &in, int w, ostream &out);
void decode (istream &in, int w, ostream &out);

#endif