#include "lz77.hpp"

void encode (istream &in, int w, ostream &out) {
	trie<int> root;
	list<string> insertions;
	int pos = 0;
	int window = 0;

	while (true) {
		int i = 0;
		char current;
		string term;
		trie<int> *node = &root;

		while (true) {
			in.get(current);
			// We peek to turn on the eof bit in case that we've reached the end
			in.peek();

			term.push_back(current);
			if (node->children.count(current) == 0 			// We don't have this character defined yet
				|| node->children[current]->value == NULL 	// The character is defined, but we don't have that term 
				|| in.eof()) {								// The term is in the dictionary, but we've reached the end
				if (i == 0) {
					out << "#0|0|" << current;
				} else {
					out << "#" << (pos - *(node->value)) << "|" << i << "|" << current;
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
			node->children[current]->value = new int(pos);
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

		if (in.eof()) break;
	}
}

token next_token (istream &in) {
	token tok;
	in.seekg(1, ios_base::cur);	// #
	in >> tok.distance;
	in.seekg(1, ios_base::cur);	// |
	in >> tok.length;
	in.seekg(1, ios_base::cur);	// |
	in >> tok.character;
	return tok;
}

void decode (istream &in, int w, ostream &out) {
	buffer<char> buff(max(1, w));

	while (true) {
		in.peek();
		if (in.eof()) break;

		token tok = next_token(in);

		for (int j = 0; j < tok.length; j++) {
			out << buff[tok.distance - 1];
			buff.insert(buff[tok.distance - 1]);
		}

		out << tok.character;
		buff.insert(tok.character);
	}
}

