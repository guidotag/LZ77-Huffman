// A trie with public internal representation. This is done in order to allow the
// compression routine traverse over the trie.
// 
// Author: Guido Tagliavini Ponce
// Date: 03/01/2015

#ifndef __TRIE_HPP__
#define __TRIE_HPP__

#include <map>
#include <string>

using std::string;
using std::map;

struct trie {
	trie (int position) {
		this->position = position;
	}

	~trie () {
		map<char, trie *>::iterator it, next;
		it = this->children.begin();

		while (it != this->children.end()) {
			next = it;
			next++;
			delete it->second;
			it = next;
		}
	}

	void insert (char c, int pos) {
		children[c] = new trie(pos);
	}

	// Deletes all the path for key[i..n], but leaving the current node.
	void erase (string &key, int i, trie *node) {
		int n = key.size();
		
		if (i == n) {
			node->position = -1;
			return;
		}

		erase(key, i + 1, node->children[key[i]]);
		
		// See if we can delete the node
		if (node->children[key[i]]->children.size() == 0
			&& node->children[key[i]]->position == -1) {
			delete node->children[key[i]];
			node->children.erase(key[i]);
		}
	}

	// Precondition: The trie must contain the key
	void erase (string &key) {
		erase(key, 0, this);
	}

	map<char, trie *> children;
	int position;
};

#endif