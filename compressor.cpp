#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "lz77.hpp"
#include "huffman.hpp"
#include "bit_writer.hpp"
#include "bit_reader.hpp"
#include "bit.hpp"

using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::string;
using std::iostream;
using std::istream;
using std::ostream;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::map;

// TODO
// - Implement dictionary as a hash table. We could use unorderered_map.

void test_lz77_stringstream (const string &text, int w) {
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

void test_lz77_filestream (const string &path, int w) {
	fstream input(path, fstream::in);
	fstream encoded("encoded.txt", fstream::out | fstream::trunc);

	encode(input, w, encoded);
	input.close();

	// Make encoded an input stream
	encoded.close();
	encoded.open("encoded.txt", fstream::in);
	noskipws(encoded);
	fstream decoded("decoded.txt", fstream::out);
	decode(encoded, w, decoded);

	encoded.close();
	decoded.close();

	system(string("diff " + path + " decoded.txt").c_str());
}

ostream &operator<<(ostream &out, code &c) {
	int n = c.size();
	for (int i = 0; i < n; i++) {
		out << c[i];
	}

	return out;
}

void test_huffman_stringstream (const string &text) {
	cout << "Text: " << text << endl;

	stringstream input;

	input << text;

	map<char, code> codes = huffman(input);
	map<char, code>::iterator it = codes.begin();
	while (it != codes.end()) {
		cout << it->first << ": " << it->second << endl;
		it++;
	}
}

void write_code (code &c, bit_writer &writer) {
	int n = c.size();
	for (int i = 0; i < n; i++) {
		writer.write_bit(c[i]);
	}
}

void write_dictionary (map<char, code> &codes, ostream &out) {
	bit_writer writer(out);

	map<char, code>::iterator it = codes.begin();
	while (it != codes.end()) {
		// Write the character
		writer.write_char(it->first);
		// Write the code length
		writer.write_char((char)it->second.size());
		// Write the code
		write_code(it->second, writer);

		it++;
	}
}

void huffmanize (istream &in, map<char, code> &codes, ostream &out) {
	bit_writer writer(out);

	char current;
	while (in >> current) {
		code c = codes[current];
		write_code(c, writer);
	}
}

void compress (ifstream &in, int w, ofstream &out) {
	fstream lz77("lz77.tmp", fstream::out | fstream::trunc);
	encode(in, w, lz77);

	lz77.close();
	lz77.open("lz77.tmp", fstream::in);

	map<char, code> codes = huffman(lz77);

	write_dictionary(codes, out);

	lz77.clear();
	lz77.seekg(0, ios::beg);

	huffmanize(lz77, codes, out);

	remove("lz77.tmp");
}

void uncompress (ifstream &in, int w, ofstream &out) {
	
}

void print_help() {

}

int main (int argc, char *argv[]) {
	// test_lz77_stringstream("ABBAABBBABAABABBABBBABAABBABBABABAABAB", 10);
	// test_lz77_filestream("data/book.txt", 1 << 2);
	// test_huffman_stringstream("Example string. The vowels should have much shorter codes than the consonants.");

	if (argc != 3) {
		print_help();
		exit(EXIT_SUCCESS);
	}

	string path(argv[1]);
	int w = atoi(argv[2]);

	ifstream in(path, fstream::in);
	ofstream out(path + ".cps", fstream::trunc);

	compress(in, w, out);

	system(string("du -k " + path).c_str());
	system(string("du -k " + path + ".cps").c_str());

	return 0;
}