#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::string;
using std::iostream;
using std::istream;
using std::ostream;
using std::fstream;
using std::stringstream;
using std::map;

extern void encode (istream &in, int w, ostream &out);
extern void decode (istream &in, int w, ostream &out);
extern map<char, string> huffman (istream &in);

// TODO
// - Implement dictionary as a hash table. We could use unorderered_map.
// - Improve the way we store the encoding tuples. Maybe using Huffman.

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

void test_huffman_stringstream (const string &text) {
	cout << "Text: " << text << endl;

	stringstream input;

	input << text;

	map<char, string> codes = huffman(input);
	map<char, string>::iterator it = codes.begin();
	while (it != codes.end()) {
		cout << it->first << ": " << it->second << endl;
		it++;
	}
}

int main (int argc, char *argv[]) {
	// test_stringstream("ABBAABBBABAABABBABBBABAABBABBABABAABAB", 10);
	// test_filestream("book.txt", 1 << 20);
	test_huffman_stringstream("Example string. The vowels should have much shorter codes than the consonants.");

	if (argc != 2) {
		print_help();
		exit(EXIT_SUCCESS);
	}

	char *path = argv[1];

	return 0;
}