#ifndef __BIT_READER_HPP__
#define __BIT_READER_HPP__

#include "bit.hpp"
#include <istream>

using std::istream;

class bit_reader {
public:
	bit_reader (istream &_in) : in(_in){
		remaining = 0;
	}

	bit read_bit () {
		if (remaining == 0) {
			char next;
			in >> next;
			unpack(next, buffer);
			remaining = 8;
		}

		bit ret = buffer[8 - remaining];
		remaining--;
		return ret;
	}

	char read_char () {
		bit array[8];
		for (int i = 0; i < 8; i++) {
			array[i] = read_bit();
		}

		return pack(array);
	}

	int peek() {
		return in.peek();
	}

	bool eof(){
		return in.eof();
	}

private:
	bit buffer[8];
	int remaining;
	istream &in;
};

#endif