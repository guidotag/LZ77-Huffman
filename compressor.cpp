#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <getopt.h>
#include <unistd.h>
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
using std::char_traits;

const char *program_name;

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
	fstream decoded("decoded.txt", fstream::out | fstream::trunc);
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

	map<int, code> codes = huffman(input);
	map<int, code>::iterator it = codes.begin();
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

void write_dictionary (map<int, code> &codes, ostream &out) {
	bit_writer writer(out);

	writer.write_int(codes.size());

	map<int, code>::iterator it = codes.begin();
	while (it != codes.end()) {
		// Write the character
		writer.write_int(it->first);
		// Write the code length
		writer.write_char((char)it->second.size());
		// Write the code
		write_code(it->second, writer);

		it++;
	}

	writer.flush();
}

map<code, int> read_dictionary (istream &in) {
	map<code, int> inverted;
	bit_reader reader(in);

	int size = reader.read_int();

	for (int i = 0; i < size; i++) {
		int character = reader.read_int();
		unsigned char len = reader.read_char();
		code c(len);
		for (int j = 0; j < len; j++) {
			c[j] = reader.read_bit();
		}

		inverted[c] = character;
	}

	return inverted;
}

void huffmanize (istream &in, ostream &out) {
	map<int, code> codes = huffman(in);
	write_dictionary(codes, out);

	in.clear();
	in.seekg(0, ios_base::beg);

	bit_writer writer(out);

	char current;
	while (in >> current) {
		code c = codes[char_traits<char>::to_int_type(current)];
		write_code(c, writer);
	}

	write_code(codes[char_traits<char>::eof()], writer);

	writer.flush();
}

void dehuffmanize (istream &in, ostream &out) {
	map<code, int> inverted = read_dictionary(in);
	bit_reader reader(in);

	while (true) {
		code current;
		while (true) {
			current.push_back(reader.read_bit());
			if (inverted.count(current) > 0) {
// cout << inverted[current] << endl;
				if (inverted[current] == char_traits<char>::eof()) return;
				out << char_traits<char>::to_char_type(inverted[current]);
				break;
			}
		}
	}
}

void compress (ifstream &in, int w, ofstream &out) {
	fstream temp("compression.tmp", fstream::out | fstream::trunc);

	temp << w;
	encode(in, w, temp);

	temp.close();
	temp.open("compression.tmp", fstream::in);
	noskipws(temp);

	huffmanize(temp, out);

	remove("compression.tmp");
}

void decompress (ifstream &in, ofstream &out) {
	fstream temp("decompression.tmp", fstream::out | fstream::trunc);

	dehuffmanize(in, temp);

	temp.close();
	temp.open("decompression.tmp", fstream::in);
	noskipws(temp);

	int w;
	temp >> w;
	decode(temp, w, out);

	remove("decompression.tmp");
}

string directory (const string &path) {
	int n = path.size();
	for (int i = n - 1; i >= 0; i--) {
		if (path[i] == '/') {
			return path.substr(0, i);
		}
	}

	return path;
}

string drop_extension (const string &path) {
	int n = path.size();
	for (int i = n - 1; i >= 0; i--) {
		if (path[i] == '/') {
			return path;		// No extension
		}
		if (path[i] == '.') {
			return path.substr(0, i);
		}
	}

	return path;
}

string filename (const string &path) {
	int n = path.size();
	for (int i = n - 1; i >= 0; i--) {
		if (path[i] == '/') {
			return path.substr(i + 1, n - i);
		}
	}

	return path;
}

void print_help() {
	printf("Usage:\n");
	printf("\t%s <options> <file>\n", program_name);
	printf("The available options are:\n");
	printf("\t-h, -help\n\t\tPrints help information.\n");
	printf("\t-c, -compress <window>\n\t\tCompresses the specified file using a dictionary of size given by window.\n");
	printf("\t-d, -decompress\n\t\tDecompresses the specified file.\n");
	printf("If no option is specified, a decompression is performed.\n");
	printf("Example calls:\n");
	printf("\t%s -c 100000 data/book.txt\n", program_name);
	printf("\t%s -d data/book.txt.lzh\n", program_name);
}

int main (int argc, char *argv[]) {
	// test_lz77_stringstream("ABBAABBBABAABABBABBBABAABBABBABABAABAB", 10);
	// test_lz77_filestream("data/joke.txt", 1 << 2);
	// test_huffman_stringstream("Example string. The vowels should have much shorter codes than the consonants.");

	program_name = argv[0];

	const char* const optstring = "hc:d";

	const struct option longops[] = {
		{"help", 0, NULL, 'h'},
		{"compress", 1, NULL, 'c'},
		{"decompress", 0, NULL, 'd'},
		{NULL, 0, NULL, 0}
	};

	if (argc < 3) {
		print_help();
		exit(EXIT_SUCCESS);
	}

	int w;
	char operation;

	while(1){
		int next_opt = getopt_long(argc, argv, optstring, longops, NULL);
		
		if(next_opt == -1){
			break;
		}
		
		switch(next_opt){
			case 'h':
				print_help();
				exit(EXIT_SUCCESS);
			case 'c':
				w = atoi(optarg);
				operation = 'c';
				break;
			case 'd':
				operation = 'd';
				break;
			default:
				abort();
		}
	}

	string path(argv[argc - 1]);

	if (operation == 'c') {
		ifstream in(path, fstream::in);
		ofstream out(path + ".lzh", fstream::out | fstream::trunc);
		noskipws(in);

		compress(in, w, out);
	
		in.close();
		out.close();
	} else {
		system(string("mkdir " + directory(path) + "/decompressed").c_str());

		ifstream in(path, fstream::in);
		ofstream out(directory(path) + "/decompressed/" + drop_extension(filename(path)), fstream::out | fstream::trunc);
		noskipws(in);

		decompress(in, out);
	
		in.close();
		out.close();
	}

	return 0;
}