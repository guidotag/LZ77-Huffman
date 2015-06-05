#include "lz77.hpp"

void encode (istream &in, wsize_t w, ostream &out) {
	trie<int> root;
	list<string> insertions;
	int pos = 0;
	int window = 0;

	while (true) {
		wsize_t i = 0;
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
					wsize_t zero = 0;
					out.write((char *)(&zero), sizeof(wsize_t));
					out.write((char *)(&zero), sizeof(wsize_t));
					out << current;
				} else {
					wsize_t dist = (wsize_t)(pos - *(node->value));
					out.write((char *)(&dist), sizeof(wsize_t));
					out.write((char *)(&i), sizeof(wsize_t));
					out << current;
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
	in.read((char *)(&tok.length), sizeof(wsize_t));
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

