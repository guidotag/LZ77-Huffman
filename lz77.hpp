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
using std::ios_base;

typedef uint16_t wsize_t;		// Window's size data type
typedef uint8_t lsize_t;		// Word's length data type

struct token {
	wsize_t distance;
	lsize_t length;
	char character;
};

void encode (istream &in, wsize_t w, ostream &out);
void decode (istream &in, wsize_t w, ostream &out);

#endif