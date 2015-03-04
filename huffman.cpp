#include <queue>
#include <vector>
#include <istream>
#include "trie.hpp"

using std::priority_queue;
using std::vector;
using std::istream;

struct tnode {
	long long weight;
	trie<char> *tr;
};

class comparator {
public:
	bool operator() (const tnode &a, const tnode &b) const {
		return a.weight > b.weight;
	}
};

void traverse (trie<char> *node, string &path, map<char, string> &codes) {
	if (node->children.size() == 0) {
		// It's a leaf
		codes[*(node->value)] = string(path);
		return;
	}

	// By construction, the node must have two children '0' and '1'
	path.push_back('0');
	traverse(node->children['0'], path, codes);
	path.pop_back();
	path.push_back('1');
	traverse(node->children['1'], path, codes);
	path.pop_back();
}

map<char, string> huffman (istream &in) {
	map<char, long long> freqs;
	char current;

	while (in >> current) {
		freqs[current]++;
	}

	// Calculate weighted frequencies while we create one
	// trie node per character.
	priority_queue<tnode, vector<tnode>, comparator> pq;
	map<char, long long>::iterator it = freqs.begin();
	while (it != freqs.end()) {
		tnode x;		// maybe we can allocate stack for this one
		x.tr = new trie<char>(it->first);
		x.weight = it->second;

		pq.push(x);

		it++;
	}

	while (pq.size() > 1) {
		tnode a, b;
		a = pq.top();
		pq.pop();
		b = pq.top();
		pq.pop();

		tnode join;
		join.tr = new trie<char>();
		join.tr->children['0'] = a.tr;
		join.tr->children['1'] = b.tr;

		join.weight = a.weight + b.weight;

		pq.push(join);
	}

	map<char, string> codes;
	string empty;
	traverse(pq.top().tr, empty, codes);

	// TODO delete the trie

	return codes;
}