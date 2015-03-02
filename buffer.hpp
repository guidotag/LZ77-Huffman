#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>

using std::vector;

template<class T>
class buffer {
	// Behaves as a FIFO queue, but allows random access to any element.
	// The structure's size is fixed at construction, and every element is
	// initialized with the type's default value.
	// The only two supported operations are element accesing and insertion.
	// Accessing the i-th element means accessing the i-th most recently inserted
	// element. We can think that each element we insert is put at the
	// front, getting the lowest index, and the last element drops off the
	// structure.

public:
	buffer<T> (int size) : array(size){
		next = size - 1;
		this->size = size;
	}

	T operator[] (size_t idx) {
		return array[(next + 1 + idx) % size];
	}

	void insert (T elem) {
		array[next] = elem;
		next--;
		if (next < 0) next += size;
	}
private:
	vector<T> array;
	int next;
	int size;
};

#endif