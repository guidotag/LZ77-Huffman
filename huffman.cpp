#include "huffman.hpp"

void traverse (trie<int> *node, code &path, map<int, code> &codes) {
	if (node->children.size() == 0) {
		// It's a leaf
		codes[*(node->value)] = code(path);
		return;
	}

	// By construction, the node must have two children '0' and '1'
	path.push_back(bit(0));
	traverse(node->children['0'], path, codes);
	path.pop_back();
	path.push_back(bit(1));
	traverse(node->children['1'], path, codes);
	path.pop_back();
}

map<int, code> huffman (istream &in) {
	map<char, long long> freqs;
	char current;

	while (in >> current) {
		freqs[current]++;
	}

	// Calculate weighted frequencies while we create one
	// trie node per character.
	priority_queue<weighted_trie, vector<weighted_trie>, comparator> pq;
	map<char, long long>::iterator it = freqs.begin();
	while (it != freqs.end()) {
		weighted_trie x;
		x.root = new trie<int>(char_traits<char>::to_int_type(it->first));
		x.weight = it->second;

		pq.push(x);

		it++;
	}

	// Add an EOF node.
	weighted_trie x;
	x.root = new trie<int>(char_traits<char>::eof());
	x.weight = 1;
	pq.push(x);

	while (pq.size() > 1) {
		weighted_trie a, b;
		a = pq.top();
		pq.pop();
		b = pq.top();
		pq.pop();

		weighted_trie join;
		join.root = new trie<int>();
		join.root->children['0'] = a.root;
		join.root->children['1'] = b.root;

		join.weight = a.weight + b.weight;

		pq.push(join);
	}

	map<int, code> codes;
	code empty;
	traverse(pq.top().root, empty, codes);

	delete pq.top().root;

	return codes;
}