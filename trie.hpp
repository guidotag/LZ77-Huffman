// A trie with internal representation left public. This is done in order to allow the
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

template<class T>
class trie {
public:	
	trie () {
		value = NULL;
	}

	trie (const T &value) {
		this->value = new T(value);
	}

	~trie () {
		typename map<char, trie<T> *>::iterator it, next;
		it = this->children.begin();

		while (it != this->children.end()) {
			next = it;
			next++;
			delete it->second;
			it = next;
		}

		if (value != NULL) delete value;
	}

	void insert (char c, T &value) {
		children[c] = new trie<T>(value);
	}

	// Deletes all the path for key[i..n], but leaving the current node.
	void erase (string &key, int i, trie<T> *node) {
		int n = key.size();
		
		if (i == n) {
			node->value = NULL;
			return;
		}

		erase(key, i + 1, node->children[key[i]]);
		
		// See if we can delete the node
		if (node->children[key[i]]->children.size() == 0
			&& node->children[key[i]]->value == NULL) {
			delete node->children[key[i]];
			node->children.erase(key[i]);
		}
	}

	// Precondition: The trie must contain the key
	void erase (string &key) {
		erase(key, 0, this);
	}

	map<char, trie<T> *> children;
	T *value;
};

#endif