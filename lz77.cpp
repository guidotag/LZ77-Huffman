#include "lz77.hpp"

const int maximum_word_length = 1 << (8 * sizeof(lsize_t));

void encode (istream &in, wsize_t w, ostream &out) {
	trie<long long> root;
	list<string> insertions;
	long long pos = 0;
	int window = 0;

	while (true) {
		int i = 0;
		char current;
		string term;
		trie<long long> *node = &root;

		while (true) {
			in.get(current);
			// We peek to turn on the eof bit in case that we've reached the end
			in.peek();

			term.push_back(current);
			if (node->children.count(current) == 0 			// We don't have this character defined yet
				|| node->children[current]->value == NULL 	// The character is defined, but we don't have that term 
				|| in.eof()									// We've reached the end
				|| i == maximum_word_length - 1) {			// We have reached the maximum word length

				if (i == 0) {
					int zero = 0;
					out.write((char *)(&zero), sizeof(wsize_t));
					out.write((char *)(&zero), sizeof(lsize_t));
					out << current;
				} else {
					wsize_t dist = (wsize_t)(pos - *(node->value));
					lsize_t length = (lsize_t)i;
					out.write((char *)(&dist), sizeof(wsize_t));
					out.write((char *)(&length), sizeof(lsize_t));
					out << current;
				}

				if (node->children.count(current) == 0) {
					// The character was not defined so we add a new node
					node->insert(current, pos);
				} else {
					// The character existed in the tree so we just set the term's position
					if (node->children[current]->value != NULL) {
						// If we already had the term, we delete it first
						delete node->children[current]->value;
						insertions.remove(term);
					}
					node->children[current]->value = new long long(pos);
				}

				break;
			} else {
				node = node->children[current];
				i++;
			}
		}

		insertions.push_back(term);

		pos += i; pos++;
		window += i; window++;

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

	in.read((char *)(&tok.distance), sizeof(wsize_t));
	in.read((char *)(&tok.length), sizeof(lsize_t));
	in >> tok.character;

	return tok;
}

void decode (istream &in, wsize_t w, ostream &out) {
	buffer<char> buff(max(1, (int)w));

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

