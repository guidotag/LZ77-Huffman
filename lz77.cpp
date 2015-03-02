#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <map>
#include "trie.hpp"
#include "buffer.hpp"

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::ios;
using std::ios_base;
using std::string;
using std::iostream;
using std::istream;
using std::ostream;
using std::fstream;
using std::stringstream;
using std::max;

struct token {
	int distance;
	int length;
	char character;
};

void encode (istream &in, int w, ostream &out) {
	trie root(-1);
	list<string> insertions;
	int pos = 0;
	int window = 0;

	out << "|";

	while (true) {
		int i = 0;
		char current;
		string term;
		trie *node = &root;

		while (true) {
			in.get(current);
			// We peek to turn on the eof bit in case that we've reached the end
			in.peek();
			term.push_back(current);
			if (node->children.count(current) == 0 			// We don't have this character defined yet
				|| node->children[current]->position == -1 	// The character is defined, but we don't have that term 
				|| in.eof()) {								// The term is in the dictionary, but we've reached the end
				if (i == 0) {
					out << "(0,0," << current << ")|";
				} else {
					out << "(" << (pos - node->position) << "," << i << "," << current << ")|";
				}
				break;
			} else {
				node = node->children[current];
				i++;
			}
		}

		if (node->children.count(current) == 0) {
			// The character was not defined so we add a new node
			node->insert(current, pos);
		} else {
			// The character existed in the tree so we just set the term's position
			node->children[current]->position = pos;
		}

		insertions.push_back(term);

		pos += i + 1;
		window += i + 1;

		while (window > w) {
			string key = insertions.front();
			insertions.pop_front();
			root.erase(key);
			window -= key.size();
		}

		if(in.eof()) break;
	}
}

token next_token (istream &in) {
	token tok;
	in.ignore(1, '(');
	in >> tok.distance;
	in.ignore(1, ',');
	in >> tok.length;
	in.ignore(1, ',');
	in >> tok.character;
	in.ignore(1, ')');
	return tok;
}

void decode (istream &in, int w, ostream &out) {
	buffer<char> buff(max(1, w));
	in.ignore(1, '|');

	while (!in.eof()) {
		token tok = next_token(in);
		in.ignore(1, '|');
		in.peek();
		for (int j = 0; j < tok.length; j++) {
			out << buff[tok.distance - 1];
			buff.insert(buff[tok.distance - 1]);
		}
		out << tok.character;
		buff.insert(tok.character);
	}
}

// TODO
// - Implement dictionary as a hash table. We could use unorderered_map.
// - Improve the way we store the encoding tuples. Maybe using Huffman.

void test_stringstream (const string &text, int w) {
	// The text should have no whitespaces (spaces, end lines, etc.)
	cout << "Original text: " << text << endl;

	stringstream input, encoded;

	input << text;
	encode(input, w, encoded);
	string enc;
	encoded >> enc;
	cout << "Encoded: " << enc << endl;

	// input.str(string());	// Clearing the stream
	stringstream decoded;

	encoded.clear();
	encoded << enc;
	decode(encoded, w, decoded);
	string dec;
	decoded >> dec;
	cout << "Decoded:" << dec << endl;
	cout.setf(ios_base::boolalpha);
	cout << "Original text and decoded are equal? " << (dec == text) << endl;
}

void test_filestream (const string &path, int w) {
	fstream input(path, fstream::in);
	fstream encoded("encoded.txt", fstream::out | fstream::trunc);	// First we open the file and delete any existing content
	encoded.close();
	encoded.open("encoded.txt", fstream::out | fstream::in);		// Then we make this an i/o stream

	encode(input, w, encoded);

	input.close();

	fstream decoded("decoded.txt", fstream::out);

	encoded.clear();
	encoded.seekg(0, ios::beg);
	noskipws(encoded);
	decode(encoded, w, decoded);
	string command("diff " + path + " decoded.txt");
	system(command.c_str());

	encoded.close();
	decoded.close();
}

int main () {
	// test_stringstream("ABBAABBBABAABABBABBBABAABBABBABABAABAB", 10);
	test_filestream("book.txt", 1 << 20);

	return 0;
}